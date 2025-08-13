#include "compiler/compiler.h"

// Standard C++ includes
#include <stack>

// Third-party includes
#include <fast_float/fast_float.h>

// GeNN includes
#include "transpiler/errorHandler.h"

// FeNN backend includes
#include "assembler/assembler.h"

using namespace GeNN;
using namespace GeNN::Transpiler;

//---------------------------------------------------------------------------
// Anonymous namespace
//---------------------------------------------------------------------------
namespace
{

//---------------------------------------------------------------------------
// CompilerError
//---------------------------------------------------------------------------
class CompilerError : public std::runtime_error
{
public:
    CompilerError() : std::runtime_error("")
    {
    }
};

void checkConversion(const Type::ResolvedType &leftType, const Type::ResolvedType &rightType, 
                     const Token &token, ErrorHandlerBase &errorHandler)
{
    const auto &leftNumeric = leftType.getNumeric();
    const auto &rightNumeric = rightType.getNumeric();

    // If either type isn't an integer and fixed point format differs
    if((!leftNumeric.isIntegral || !rightNumeric.isIntegral)
        && (!leftNumeric.fixedPoint || leftNumeric.fixedPoint != rightNumeric.fixedPoint))
    {
        errorHandler.error(token, "FeNN only currently supports assignement, addition and subtraction "
                                  "of numbers in same fixed point/integer format");
        throw CompilerError();
    }
}

bool isSaturating(const Type::ResolvedType &aType, const Type::ResolvedType &bType)
{
    return (aType.getNumeric().isSaturating || bType.getNumeric().isSaturating);
}

int getConversionShift(const Type::ResolvedType &resultType, const Type::ResolvedType &leftType, const Type::ResolvedType &rightType,
                       const Token &token, ErrorHandlerBase &errorHandler)
{
    const int resultFixedPoint = resultType.getNumeric().fixedPoint.value_or(0);
    const int leftFixedPoint = leftType.getNumeric().fixedPoint.value_or(0);
    const int rightFixedPoint = rightType.getNumeric().fixedPoint.value_or(0);

    if(resultFixedPoint == leftFixedPoint) {
        return rightFixedPoint;
    }
    else if (resultFixedPoint == rightFixedPoint) {
        return leftFixedPoint;
    }
    else {
        errorHandler.error(token, "Invalid fixed point types for conversion shift");
        throw CompilerError();
    }
}

//---------------------------------------------------------------------------
// Visitor
//---------------------------------------------------------------------------
class Visitor : public Expression::Visitor, public Statement::Visitor
{
public:
    Visitor(const Statement::StatementList &statements, EnvironmentInternal &environment,
            const Type::TypeContext &context, const TypeChecker::ResolvedTypeMap &resolvedTypes,
            ErrorHandlerBase &errorHandler, const std::unordered_map<int16_t, VectorRegisterAllocator::RegisterPtr> &literalPool,
            ScalarRegisterAllocator::RegisterPtr maskRegister,
            ScalarRegisterAllocator &scalarRegisterAllocator, VectorRegisterAllocator &vectorRegisterAllocator)
    :   m_Environment(environment), m_Context(context), m_MaskRegister(maskRegister), m_ResolvedTypes(resolvedTypes),
        m_ErrorHandler(errorHandler), m_LiteralPool(literalPool), m_ScalarRegisterAllocator(scalarRegisterAllocator), 
        m_VectorRegisterAllocator(vectorRegisterAllocator)
    {
         for(auto &s : statements) {
            s.get()->accept(*this);
         }
    }

    //---------------------------------------------------------------------------
    // Public API
    //---------------------------------------------------------------------------
    RegisterPtr getExpressionRegister() const
    {
        return m_ExpressionRegister.value().first;
    }

private:
    //---------------------------------------------------------------------------
    // Expression::Visitor virtuals
    //---------------------------------------------------------------------------
    virtual void visit(const Expression::ArraySubscript &arraySubscript) final
    {
        assert(false);
    }

    virtual void visit(const Expression::Assignment &assignement) final
    {
        const auto vecAssigneeReg = getExpressionVectorRegister(assignement.getAssignee());
        const auto vecValueReg = getExpressionVectorRegister(assignement.getValue());
        const auto &assigneeType = m_ResolvedTypes.at(assignement.getAssignee());
        const auto &valueType = m_ResolvedTypes.at(assignement.getValue());

        // If a mask is set
        // **TODO** only necessary when assigning to variables outside of masked scope
        const auto &opToken = assignement.getOperator();
        if(m_MaskRegister) {
            // If we're doing plain assignement, conditionally assign from value register directly
            if(opToken.type == Token::Type::EQUAL) {
                m_Environment.get().getCodeGenerator().vsel(*vecAssigneeReg, *m_MaskRegister, *vecValueReg);
            }
            // Otherwise
            else {
                // Generate assignement into temporary register
                // **TODO** if value reg is reusable, no real need for extra register
                const auto tempReg = m_VectorRegisterAllocator.getRegister();
                generateAssign(opToken, *tempReg, *vecAssigneeReg, *vecValueReg,
                               assigneeType, valueType);

                // Conditionally assign back to assignee register
                m_Environment.get().getCodeGenerator().vsel(*vecAssigneeReg, *m_MaskRegister, *tempReg);
            }
        }
        // Otherwise, generate assignement directly into assignee register
        else {
            generateAssign(opToken, *vecAssigneeReg, *vecAssigneeReg, *vecValueReg,
                           assigneeType, valueType);
        }
    }

    virtual void visit(const Expression::Binary &binary) final
    {
        const auto opType = binary.getOperator().type;
        const auto &leftType = m_ResolvedTypes.at(binary.getLeft());
        const auto &rightType = m_ResolvedTypes.at(binary.getRight());

        // If operation is bitwise
        if(opType == Token::Type::AMPERSAND || opType == Token::Type::CARET || opType == Token::Type::PIPE) {
            const auto scalarLeftReg = getExpressionScalarRegister(binary.getLeft());
            const auto scalarRightReg = getExpressionScalarRegister(binary.getRight());

            const auto resultReg = m_ScalarRegisterAllocator.getRegister();
            if(opType == Token::Type::AMPERSAND) {
                m_Environment.get().getCodeGenerator().and_(*resultReg, *scalarLeftReg, *scalarRightReg);
            }
            else if(opType == Token::Type::CARET) {
                m_Environment.get().getCodeGenerator().xor_(*resultReg, *scalarLeftReg, *scalarRightReg);
            }
            else if(opType == Token::Type::PIPE) {
                m_Environment.get().getCodeGenerator().or_(*resultReg, *scalarLeftReg, *scalarRightReg);
            }
            
            // Set result register
            // **NOTE** result is a temporary register so always re-usable
            setExpressionRegister(resultReg, true);
        }
        else {
            const auto vecLeftReg = getExpressionVectorRegister(binary.getLeft());
            const auto vecRightReg = getExpressionVectorRegister(binary.getRight());

            // If operation is arithmetic
            if(opType == Token::Type::MINUS || opType == Token::Type::PLUS || opType == Token::Type::STAR) {
                const auto resultReg = m_VectorRegisterAllocator.getRegister();
                if(opType == Token::Type::MINUS) {
                    checkConversion(leftType, rightType, binary.getOperator(), m_ErrorHandler.get());

                    if(isSaturating(leftType, rightType)) {
                        m_Environment.get().getCodeGenerator().vsub_s(*resultReg, *vecLeftReg, *vecRightReg);
                    }
                    else {
                        m_Environment.get().getCodeGenerator().vsub(*resultReg, *vecLeftReg, *vecRightReg);
                    }
                }
                else if(opType == Token::Type::PLUS) {
                    checkConversion(leftType, rightType, binary.getOperator(), m_ErrorHandler.get());

                    if(isSaturating(leftType, rightType)) {
                        m_Environment.get().getCodeGenerator().vadd_s(*resultReg, *vecLeftReg, *vecRightReg);
                    }
                    else {
                        m_Environment.get().getCodeGenerator().vadd(*resultReg, *vecLeftReg, *vecRightReg);
                    }
                }
                else if(opType == Token::Type::STAR) {
                    const auto &resultType = m_ResolvedTypes.at(&binary);

                    // **TODO** rounding
                    const int shift = getConversionShift(resultType, leftType, rightType, 
                                                         binary.getOperator(), m_ErrorHandler.get());
                    m_Environment.get().getCodeGenerator().vmul(shift, *resultReg, *vecLeftReg, *vecRightReg);
                }
            
                // Set result register
                // **NOTE** result is a temporary register so always re-usable
                setExpressionRegister(resultReg, true);
            }
            // Otherwise, if it is relational
            else if(opType == Token::Type::GREATER || opType == Token::Type::GREATER_EQUAL || opType == Token::Type::LESS
                    || opType == Token::Type::LESS_EQUAL || opType == Token::Type::NOT_EQUAL || opType == Token::Type::EQUAL_EQUAL) 
            {
                const auto resultReg = m_ScalarRegisterAllocator.getRegister();
                checkConversion(leftType, rightType, binary.getOperator(), m_ErrorHandler.get());

                if(opType == Token::Type::GREATER) {
                    m_Environment.get().getCodeGenerator().vtlt(*resultReg, *vecRightReg, *vecLeftReg);
                }
                else if(opType == Token::Type::GREATER_EQUAL) {
                    m_Environment.get().getCodeGenerator().vtge(*resultReg, *vecLeftReg, *vecRightReg);
                }
                else if(opType == Token::Type::LESS) {
                    m_Environment.get().getCodeGenerator().vtlt(*resultReg, *vecLeftReg, *vecRightReg);
                }
                else if(opType == Token::Type::LESS_EQUAL) {
                    m_Environment.get().getCodeGenerator().vtge(*resultReg, *vecRightReg, *vecLeftReg);
                }
                else if(opType == Token::Type::NOT_EQUAL) {
                    m_Environment.get().getCodeGenerator().vtne(*resultReg, *vecLeftReg, *vecRightReg);
                }
                else if(opType == Token::Type::EQUAL_EQUAL) {
                    m_Environment.get().getCodeGenerator().vteq(*resultReg, *vecLeftReg, *vecRightReg);
                }
            
                // Set result register
                // **NOTE** result is a temporary register so always re-usable
                setExpressionRegister(resultReg, true);
            }
            else {
                m_ErrorHandler.get().error(binary.getOperator(), "Unsupported binary operator");
                throw CompilerError();
            }
        }
    }

    virtual void visit(const Expression::Call &call) final
    {
        // Cache reference to current reference
        std::reference_wrapper<EnvironmentBase> oldEnvironment = m_Environment; 

        // Loop through call arguments
        std::vector<RegisterPtr> arguments;
        arguments.reserve(call.getArguments().size());
        for (const auto &a : call.getArguments()) {
            // Create new environment and set to current
            EnvironmentInternal environment(oldEnvironment.get());
            m_Environment = environment;

            // Compile expression in this environment 
            // and add result register to vector
            a->accept(*this);
            arguments.push_back(getExpressionRegister());
        }

        // Restore old environment
        m_Environment = oldEnvironment;
        
        // Push arguments to top of stack
        m_CallArguments.emplace(arguments);
        
        // Compile callee
        call.getCallee()->accept(*this);

        // Pop stack
        m_CallArguments.pop();
    }

    virtual void visit(const Expression::Cast &cast) final
    {
        // **TODO** casting to/from fixed point performs shift
        // **NOTE** allow expression register to pass through
        cast.getExpression()->accept(*this);
    }

    virtual void visit(const Expression::Conditional &conditional) final
    {
        const auto conditionReg = getExpressionScalarRegister(conditional.getCondition());
        const auto trueReg = getExpressionVectorRegister(conditional.getTrue());
        const auto falseReg = getExpressionVectorRegister(conditional.getFalse());
        assert(isExpressionRegisterReusable());

        m_Environment.get().getCodeGenerator().vsel(*falseReg, *conditionReg, *trueReg);

        // Set result register
        setExpressionRegister(falseReg, true);
    }

    virtual void visit(const Expression::Grouping &grouping) final
    {
        // **NOTE** allow expression register to pass through
        grouping.getExpression()->accept(*this);
    }

    virtual void visit(const Expression::Literal &literal) final
    {
        const auto lexeme = literal.getValue().lexeme;
        const char *lexemeBegin = lexeme.c_str();
        const char *lexemeEnd = lexemeBegin + lexeme.size();
        
        // If literal is a number
        int64_t integerResult;
        if(literal.getValue().type == Token::Type::NUMBER) {
            // If it is an integer
            const auto &numericType = m_ResolvedTypes.at(&literal).getNumeric();
            if(numericType.isIntegral) {
                if(numericType.isSigned) {
                    int result;
                    auto answer = fast_float::from_chars(lexemeBegin, lexemeEnd, result);
                    assert(answer.ec == std::errc());
                    integerResult = result;
                }
                else {
                    unsigned int result;
                    auto answer = fast_float::from_chars(lexemeBegin, lexemeEnd, result);
                    assert(answer.ec == std::errc());
                    integerResult = result;
                }
            }
            // Otherwise, if it is fixed point
            else if(numericType.fixedPoint) {
                float result;
                auto answer = fast_float::from_chars(lexemeBegin, lexemeEnd, result);
                assert(answer.ec == std::errc());
                integerResult = static_cast<int64_t>(std::round(result * (1u << numericType.fixedPoint.value())));
            }
            else {
                m_ErrorHandler.get().error(literal.getValue(), "FeNN does not support floating point types");
                throw CompilerError();
            }
        }
        else {
            m_ErrorHandler.get().error(literal.getValue(), "Unsupported literal type");
            throw CompilerError();
        }

        assert(integerResult >= std::numeric_limits<int16_t>::min());
        assert(integerResult <= std::numeric_limits<int16_t>::max());

        // Set result register
        // **NOTE** result is a register assigned to pool so shouldn't be re-used
        setExpressionRegister(m_LiteralPool.at(static_cast<int16_t>(integerResult)), false);
    }

    virtual void visit(const Expression::Logical &logical) final
    {
        const auto scalarLeftReg = getExpressionScalarRegister(logical.getLeft());
        const auto scalarRightReg = getExpressionScalarRegister(logical.getRight());

        // **TODO** short-circuiting
        const auto resultReg = m_ScalarRegisterAllocator.getRegister();
        if(logical.getOperator().type == Token::Type::AMPERSAND_AMPERSAND) {
            m_Environment.get().getCodeGenerator().and_(*resultReg, *scalarLeftReg, *scalarRightReg);
        }
        else if(logical.getOperator().type == Token::Type::PIPE_PIPE){
            m_Environment.get().getCodeGenerator().or_(*resultReg, *scalarLeftReg, *scalarRightReg);
        }
        else {
            assert(false);
        }

        // Set result register
        // **NOTE** result is a temporary register so always re-usable
        setExpressionRegister(resultReg, true);
    }

    virtual void visit(const Expression::PostfixIncDec &postfixIncDec) final
    {
        const auto vecTargetReg = getExpressionVectorRegister(postfixIncDec.getTarget());
        const auto &targetType = m_ResolvedTypes.at(postfixIncDec.getTarget());

        // Make a copy of initial value
        const auto copyReg = m_VectorRegisterAllocator.getRegister();
        generateVMOV(*copyReg, *vecTargetReg);
        
        const auto &opToken = postfixIncDec.getOperator();
        if (m_MaskRegister) {
            // Generate increment/decrement into temporary register
            // **TODO** if value reg is reusable, no real need for extra register
            const auto tempReg = m_VectorRegisterAllocator.getRegister();
            generateIncDec(opToken, *tempReg, *vecTargetReg, targetType);

            // Conditionally assign back to assignee register
            m_Environment.get().getCodeGenerator().vsel(*vecTargetReg, *m_MaskRegister, *tempReg);
        }
        // Otherwise, generate assignement directly into assignee register
        else {
            generateIncDec(opToken, *vecTargetReg, *vecTargetReg, targetType);
        }

        // Return copy of initial value
        // **NOTE** result is a temporary register so always re-usable
        setExpressionRegister(copyReg, true);
    }

    virtual void visit(const Expression::PrefixIncDec &prefixIncDec) final
    {
        const auto vecTargetReg = getExpressionVectorRegister(prefixIncDec.getTarget());
        const auto &targetType = m_ResolvedTypes.at(prefixIncDec.getTarget());

        const auto &opToken = prefixIncDec.getOperator();
        if (m_MaskRegister) {
            // Generate increment/decrement into temporary register
            // **TODO** if value reg is reusable, no real need for extra register
            const auto tempReg = m_VectorRegisterAllocator.getRegister();
            generateIncDec(opToken, *tempReg, *vecTargetReg, targetType);

            // Conditionally assign back to assignee register
            m_Environment.get().getCodeGenerator().vsel(*vecTargetReg, *m_MaskRegister, *tempReg);
        }
        // Otherwise, generate assignement directly into assignee register
        else {
            generateIncDec(opToken, *vecTargetReg, *vecTargetReg, targetType);
        }

        setExpressionRegister(vecTargetReg, false);
    }

    virtual void visit(const Expression::Identifier &identifier) final
    {
        // Get type of identifier
        const auto &type = m_ResolvedTypes.at(&identifier);

        // If identifier is function i.e. name is a function template
        if (type.isFunction()) {
            // Check that there are call arguments on the stack
            assert(!m_CallArguments.empty());

            // Check argument sizes match
            assert(type.getFunction().hasFlag(Type::FunctionFlags::VARIADIC)
                   || m_CallArguments.top().size() == type.getFunction().argTypes.size());

            // Get function generator
            const auto functionGenerator = m_Environment.get().getFunctionGenerator(identifier.getName().lexeme,
                                                                                    type);

            // Cache reference to current reference
            std::reference_wrapper<EnvironmentBase> oldEnvironment = m_Environment; 
            {
                // Create new environment and set to current
                EnvironmentInternal environment(m_Environment);
                m_Environment = environment;
                
                // Call function generator to generate code
                const auto result = functionGenerator(m_Environment.get(), m_VectorRegisterAllocator, 
                                                      m_ScalarRegisterAllocator, m_MaskRegister, m_CallArguments.top());
                setExpressionRegister(result.first, result.second);
            }

            // Restore old environment
            m_Environment = oldEnvironment;
          
        }
        // Otherwise, if there are array subscript arguments on top of stack
        else if(!m_CallArguments.empty()) {
            assert(m_CallArguments.top().size() == 1);
            assert(false);
        }
        else {
            // Set result register
            // **NOTE** we don't want to re-use registers used for variables
            setExpressionRegister(m_Environment.get().getRegister(identifier.getName().lexeme), false);
        }
    }

    virtual void visit(const Expression::Unary &unary) final
    {
        // If operation is negation
        if(unary.getOperator().type == Token::Type::MINUS) {
            // Negate by subtracting from zero
            const auto vecTargetReg = getExpressionVectorRegister(unary.getRight());
            const auto resultReg = m_VectorRegisterAllocator.getRegister();
            m_Environment.get().getCodeGenerator().vsub(*resultReg, *m_Environment.get().getVectorRegister("_zero"), *vecTargetReg);
            
            // Return result
            // **NOTE** result is a temporary register so always re-usable
            setExpressionRegister(resultReg, true);
        }
        // Otherwise, if operation isn't PLUS (which can be ignored), give error
        else if(unary.getOperator().type == Token::Type::PLUS) {
            assert(false);
        }
    }

    //---------------------------------------------------------------------------
    // Statement::Visitor virtuals
    //---------------------------------------------------------------------------
    virtual void visit(const Statement::Break&) final
    {
        assert(false);
    }

    virtual void visit(const Statement::Compound &compound) final
    {
        // Cache reference to current reference
        std::reference_wrapper<EnvironmentBase> oldEnvironment = m_Environment; 

        // Create new environment and set to current
        EnvironmentInternal environment(m_Environment);
        m_Environment = environment;

        for(auto &s : compound.getStatements()) {
            s->accept(*this);
        }

        // Restore old environment
        m_Environment = oldEnvironment;
    }

    virtual void visit(const Statement::Continue&) final
    {
        assert(false);
    }

    virtual void visit(const Statement::Do &doStatement) final
    {
        // Backup mask register
        auto oldMaskRegister = m_MaskRegister;

        // Allocate mask register
        m_MaskRegister = m_ScalarRegisterAllocator.getRegister();

        // If we already have a mask register, copy it into new register
        if(oldMaskRegister) {
            m_Environment.get().getCodeGenerator().mv(*m_MaskRegister, *oldMaskRegister);
        }
        // Otherwise, load mask register with FFFF
        else {
            m_Environment.get().getCodeGenerator().li(*m_MaskRegister, 0xFFFFFFFF);
        }

        // Start loop
        Label doLoop;
        m_Environment.get().getCodeGenerator().L(doLoop);
        {
            // Generate body
            doStatement.getBody()->accept(*this);

            // And mask register with result of evaluating condition
            m_Environment.get().getCodeGenerator().and_(*m_MaskRegister, *m_MaskRegister, 
                                                        *getExpressionScalarRegister(doStatement.getCondition()));

            // If mask isn't entirely zeroed yet, goto loop
            m_Environment.get().getCodeGenerator().bne(*m_MaskRegister, Reg::X0, doLoop);
        }

        // Restore old mask register
        m_MaskRegister = oldMaskRegister;
    }

    virtual void visit(const Statement::Expression &expression) final
    {
        if(expression.getExpression()) {
            expression.getExpression()->accept(*this);
        }
    }

    virtual void visit(const Statement::For &forStatement) final
    {
        assert(false);
        // Cache reference to current reference
        /*std::reference_wrapper<EnvironmentBase> oldEnvironment = m_Environment; 

        // Create new environment and set to current
        EnvironmentInternal environment(m_Environment);
        m_Environment = environment;

        m_Environment.get().getStream() << "for(";
        if(forStatement.getInitialiser()) {
            forStatement.getInitialiser()->accept(*this);
        }
        else {
            m_Environment.get().getStream() << ";";
        }
        m_Environment.get().getStream() << " ";

        if(forStatement.getCondition()) {
            forStatement.getCondition()->accept(*this);
        }

        m_Environment.get().getStream() << "; ";
        if(forStatement.getIncrement()) {
            forStatement.getIncrement()->accept(*this);
        }
        m_Environment.get().getStream() << ")";
        forStatement.getBody()->accept(*this);

        // Restore old environment
        m_Environment = oldEnvironment;*/
    }

    virtual void visit(const Statement::ForEachSynapse &forEachSynapseStatement) final
    {
        assert(false);
    }

    virtual void visit(const Statement::If &ifStatement) final
    {
        const auto scalarConditionReg = getExpressionScalarRegister(ifStatement.getCondition());

        // If we already have a mask register, AND it with result of condition into new register
        auto oldMaskRegister = m_MaskRegister;
        if(oldMaskRegister) {
            auto combinedMaskRegister = m_ScalarRegisterAllocator.getRegister();
            m_Environment.get().getCodeGenerator().and_(*combinedMaskRegister, *oldMaskRegister, *scalarConditionReg);
            m_MaskRegister = combinedMaskRegister;
        }
        // Otherwise, just
        else {
            m_MaskRegister = scalarConditionReg;
        }

        ifStatement.getThenBranch()->accept(*this);


        if(ifStatement.getElseBranch()) {
            // Negate mask
            auto elseMaskRegister = m_ScalarRegisterAllocator.getRegister();
            m_Environment.get().getCodeGenerator().not_(*elseMaskRegister, *scalarConditionReg);

            // If we have an old mask, and it with this
            if(oldMaskRegister) {
                m_Environment.get().getCodeGenerator().and_(*elseMaskRegister, *oldMaskRegister,
                                                            *elseMaskRegister);
            }

            m_MaskRegister = elseMaskRegister;

            ifStatement.getElseBranch()->accept(*this);
        }

        // Restore old mask register
        m_MaskRegister = oldMaskRegister;
    }

    virtual void visit(const Statement::Labelled &labelled) final
    {
        assert(false);
    }

    virtual void visit(const Statement::Switch &switchStatement) final
    {
        assert(false);
    }

    virtual void visit(const Statement::VarDeclaration &varDeclaration) final
    {
        const size_t numDeclarators = varDeclaration.getInitDeclaratorList().size();
        for(size_t i = 0; i < numDeclarators; i++) {
            const auto &var = varDeclaration.getInitDeclaratorList()[i];

            // If variable is assigned to something
            if(std::get<1>(var)) {
                // Evaluate value
                const auto valueVectorReg = getExpressionVectorRegister(std::get<1>(var).get());

                // If value register can be re-used, poach it for variable
                if(isExpressionRegisterReusable()) {
                    m_Environment.get().define(std::get<0>(var).lexeme, valueVectorReg);
                }
                else {
                    // Allocate register and define variable to it
                    const auto varReg = m_VectorRegisterAllocator.getRegister(std::get<0>(var).lexeme.c_str());
                    m_Environment.get().define(std::get<0>(var).lexeme, varReg);
                    
                    // Copy value into result
                    // **NOTE** mask never matters here as these variables are inherantly local
                    generateVMOV(*varReg, *valueVectorReg);
                }
            }
            // Otherwise, allocate register and define variable to it
            else {
                const auto varReg = m_VectorRegisterAllocator.getRegister(std::get<0>(var).lexeme.c_str());
                m_Environment.get().define(std::get<0>(var).lexeme, varReg);
            }
        }
    }

    virtual void visit(const Statement::While &whileStatement) final
    {
        // Backup mask register
        auto oldMaskRegister = m_MaskRegister;

        // Allocate mask register
        m_MaskRegister = m_ScalarRegisterAllocator.getRegister();

        // If we already have a mask register, copy it into new register
        if(oldMaskRegister) {
            m_Environment.get().getCodeGenerator().mv(*m_MaskRegister, *oldMaskRegister);
        }
        // Otherwise, load mask register with FFFF
        else {
            m_Environment.get().getCodeGenerator().li(*m_MaskRegister, 0xFFFFFFFF);
        }

        // Start loop
        Label whileLoopStart;
        Label whileLoopEnd;
        m_Environment.get().getCodeGenerator().L(whileLoopStart);
        {
            // And mask register with result of evaluating condition
            m_Environment.get().getCodeGenerator().and_(*m_MaskRegister, *m_MaskRegister, 
                                                        *getExpressionScalarRegister(whileStatement.getCondition()));

            // If mask is zeroed, leave loop
            m_Environment.get().getCodeGenerator().beq(*m_MaskRegister, Reg::X0, whileLoopEnd);

            // Generate body
            whileStatement.getBody()->accept(*this);

            // Go back to start of loop
            m_Environment.get().getCodeGenerator().j_(whileLoopStart);
        }

        m_Environment.get().getCodeGenerator().L(whileLoopEnd);

        // Restore old mask register
        m_MaskRegister = oldMaskRegister;
    }

    //---------------------------------------------------------------------------
    // Private methods
    //---------------------------------------------------------------------------
    bool isExpressionRegisterReusable() const
    {
        return m_ExpressionRegister.value().second;
    }

    void setExpressionRegister()
    {
        m_ExpressionRegister = std::nullopt;
    }

    void setExpressionRegister(RegisterPtr reg, bool reusable)
    {
        m_ExpressionRegister = std::make_pair(reg, reusable);
    }

    VectorRegisterAllocator::RegisterPtr getExpressionVectorRegister(const Expression::Base *expression)
    {
        expression->accept(*this);

        return std::get<VectorRegisterAllocator::RegisterPtr>(getExpressionRegister());
    }

    ScalarRegisterAllocator::RegisterPtr getExpressionScalarRegister(const Expression::Base *expression)
    {
        expression->accept(*this);

        return std::get<ScalarRegisterAllocator::RegisterPtr>(getExpressionRegister());
    }

    void generateVMOV(VReg destinationReg, VReg sourceReg) const
    {
        m_Environment.get().getCodeGenerator().vadd(destinationReg, sourceReg,
                                                    *m_Environment.get().getVectorRegister("_zero"));
    }

    void generateAssign(const Token &token, VReg destinationReg, VReg assigneeReg, VReg valueReg,
                        const Type::ResolvedType &assigneeType, const Type::ResolvedType &valueType)
    {
        if(token.type == Token::Type::EQUAL) {
            checkConversion(assigneeType, valueType, token, m_ErrorHandler.get());
            generateVMOV(destinationReg, valueReg);
        }
        else if(token.type == Token::Type::STAR_EQUAL) {
            m_Environment.get().getCodeGenerator().vmul(valueType.getNumeric().fixedPoint.value_or(0),
                                                        destinationReg, assigneeReg, valueReg);
        }
        else if(token.type == Token::Type::PLUS_EQUAL) {
            checkConversion(assigneeType, valueType, token, m_ErrorHandler.get());
            if(isSaturating(assigneeType, valueType)) {
                m_Environment.get().getCodeGenerator().vadd_s(destinationReg, assigneeReg, valueReg);
            }
            else {
                m_Environment.get().getCodeGenerator().vadd(destinationReg, assigneeReg, valueReg);
            }
        }
        else if(token.type == Token::Type::MINUS_EQUAL) {
            checkConversion(assigneeType, valueType, token, m_ErrorHandler.get());
            if(isSaturating(assigneeType, valueType)) {
                m_Environment.get().getCodeGenerator().vsub_s(destinationReg, assigneeReg, valueReg);
            }
            else {
                m_Environment.get().getCodeGenerator().vsub(destinationReg, assigneeReg, valueReg);
            }
        }
        else {
            m_ErrorHandler.get().error(token, "Unsupported assignement operator");
            throw CompilerError();
        }
    }

    void generateIncDec(const Token &token, VReg destinationReg, VReg targetReg, const Type::ResolvedType &targetType)
    {
        // If target is integer, load integer 1
        const auto oneReg = m_VectorRegisterAllocator.getRegister();
        if (targetType.isNumeric() && targetType.getNumeric().isIntegral) {
            m_Environment.get().getCodeGenerator().vlui(*oneReg, 1);
        }
        // Otherwise, if target is fixed point, load fixed-point 1
        else if (targetType.isNumeric() && targetType.getNumeric().fixedPoint) {
            m_Environment.get().getCodeGenerator().vlui(*oneReg, 1u << targetType.getNumeric().fixedPoint.value());
        }
        else {
            m_ErrorHandler.get().error(token, "Unsupported increment/decrement target type");
            throw CompilerError();
        }

        if (token.type == Token::Type::PLUS_PLUS) {
            if (targetType.getNumeric().isSaturating) {
                m_Environment.get().getCodeGenerator().vadd_s(destinationReg, targetReg, *oneReg);
            }
            else {
                m_Environment.get().getCodeGenerator().vadd(destinationReg, targetReg, *oneReg);
            }
        }
        else if (token.type == Token::Type::MINUS_MINUS) {
            if (targetType.getNumeric().isSaturating) {
                m_Environment.get().getCodeGenerator().vsub_s(destinationReg, targetReg, *oneReg);
            }
            else {
                m_Environment.get().getCodeGenerator().vsub(destinationReg, targetReg, *oneReg);
            }
        }
        else {
            m_ErrorHandler.get().error(token, "Unsupported increment/decrement operator");
            throw CompilerError();
        }
    }

    
    //---------------------------------------------------------------------------
    // Members
    //---------------------------------------------------------------------------
    std::reference_wrapper<EnvironmentBase> m_Environment;
    const Type::TypeContext &m_Context;
    std::optional<std::pair<RegisterPtr, bool>> m_ExpressionRegister;
    std::stack<std::vector<RegisterPtr>> m_CallArguments;

    ScalarRegisterAllocator::RegisterPtr m_MaskRegister;
    const TypeChecker::ResolvedTypeMap &m_ResolvedTypes;
    std::reference_wrapper<ErrorHandlerBase> m_ErrorHandler;
    const std::unordered_map<int16_t, VectorRegisterAllocator::RegisterPtr> &m_LiteralPool;
    ScalarRegisterAllocator &m_ScalarRegisterAllocator;
    VectorRegisterAllocator &m_VectorRegisterAllocator;
};
}

//---------------------------------------------------------------------------
// GeNN::CodeGenerator::FeNN::Compiler::EnvironmentInternal
//---------------------------------------------------------------------------
void EnvironmentInternal::define(const std::string &name, RegisterPtr reg)
{
    if(!m_LocalVariables.emplace(name, reg).second) {
        throw std::runtime_error("Redeclaration of variable");
    }
}
//----------------------------------------------------------------------------
RegisterPtr EnvironmentInternal::getRegister(const std::string &name)
{
    auto l = m_LocalVariables.find(name);
    if(l == m_LocalVariables.end()) {
        return m_Enclosing.getRegister(name);
    }
    else {
        return l->second;
    }
}
//----------------------------------------------------------------------------
FunctionGenerator EnvironmentInternal::getFunctionGenerator(const std::string &name, std::optional<Type::ResolvedType> type)
{
    return m_Enclosing.getFunctionGenerator(name, type);
}
//----------------------------------------------------------------------------
CodeGenerator &EnvironmentInternal::getCodeGenerator()
{
    return m_Enclosing.getCodeGenerator();
}
//----------------------------------------------------------------------------
void compile(const Statement::StatementList &statements, EnvironmentInternal &environment, 
             const Type::TypeContext &context, const TypeChecker::ResolvedTypeMap &resolvedTypes,
             ErrorHandlerBase &errorHandler, const std::unordered_map<int16_t, VectorRegisterAllocator::RegisterPtr> &literalPool,
             ScalarRegisterAllocator::RegisterPtr maskRegister, 
             ScalarRegisterAllocator &scalarRegisterAllocator, VectorRegisterAllocator &vectorRegisterAllocator)
{
    Visitor visitor(statements, environment, context, resolvedTypes, errorHandler, literalPool, maskRegister,
                    scalarRegisterAllocator, vectorRegisterAllocator);
}