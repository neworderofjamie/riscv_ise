/*
  This file contains docstrings for use in the Python bindings.
  Do not edit! They were automatically extracted by pybind11_mkdoc.
 */

#define __EXPAND(x)                                      x
#define __COUNT(_1, _2, _3, _4, _5, _6, _7, COUNT, ...)  COUNT
#define __VA_SIZE(...)                                   __EXPAND(__COUNT(__VA_ARGS__, 7, 6, 5, 4, 3, 2, 1))
#define __CAT1(a, b)                                     a ## b
#define __CAT2(a, b)                                     __CAT1(a, b)
#define __DOC1(n1)                                       __doc_##n1
#define __DOC2(n1, n2)                                   __doc_##n1##_##n2
#define __DOC3(n1, n2, n3)                               __doc_##n1##_##n2##_##n3
#define __DOC4(n1, n2, n3, n4)                           __doc_##n1##_##n2##_##n3##_##n4
#define __DOC5(n1, n2, n3, n4, n5)                       __doc_##n1##_##n2##_##n3##_##n4##_##n5
#define __DOC6(n1, n2, n3, n4, n5, n6)                   __doc_##n1##_##n2##_##n3##_##n4##_##n5##_##n6
#define __DOC7(n1, n2, n3, n4, n5, n6, n7)               __doc_##n1##_##n2##_##n3##_##n4##_##n5##_##n6##_##n7
#define DOC(...)                                         __EXPAND(__EXPAND(__CAT2(__DOC, __VA_SIZE(__VA_ARGS__)))(__VA_ARGS__))

#if defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif


static const char *__doc_Common_Barrier = R"doc()doc";

static const char *__doc_Common_Barrier_Barrier = R"doc()doc";

static const char *__doc_Common_Barrier_Barrier_2 = R"doc()doc";

static const char *__doc_Common_Barrier_m_Cond = R"doc()doc";

static const char *__doc_Common_Barrier_m_Count = R"doc()doc";

static const char *__doc_Common_Barrier_m_Generation = R"doc()doc";

static const char *__doc_Common_Barrier_m_Mutex = R"doc()doc";

static const char *__doc_Common_Barrier_m_ResetCount = R"doc()doc";

static const char *__doc_Common_Barrier_wait = R"doc()doc";

static const char *__doc_Common_Barrier_waitAndDrop = R"doc()doc";

static const char *__doc_Common_Barrier_waitInternal = R"doc()doc";

static const char *__doc_Common_Logging_Channel = R"doc()doc";

static const char *__doc_Common_Logging_Channel_CHANNEL_COMPILER_FRONTEND = R"doc()doc";

static const char *__doc_Common_Logging_Channel_CHANNEL_FRONTEND = R"doc()doc";

static const char *__doc_Common_Logging_Channel_CHANNEL_MAX = R"doc()doc";

static const char *__doc_Common_Logging_init = R"doc()doc";

static const char *__doc_Common_Utils = R"doc()doc";

static const char *__doc_Common_Utils_Overload = R"doc(Boilerplate for overloading base std::visit)doc";

static const char *__doc_Common_Utils_SHA1Hash = R"doc(Functor for generating a hash suitable for use in std::unordered_map etc (i.e. size_t size) from a SHA1 digests)doc";

static const char *__doc_Common_Utils_SHA1Hash_operator_call = R"doc()doc";

static const char *__doc_Common_Utils_breakPoint = R"doc()doc";

static const char *__doc_Common_Utils_ceilDivide = R"doc(Divide two integers, rounding up i.e. effectively taking ceil)doc";

static const char *__doc_Common_Utils_clz = R"doc()doc";

static const char *__doc_Common_Utils_convertFixedPoint = R"doc()doc";

static const char *__doc_Common_Utils_ctz = R"doc()doc";

static const char *__doc_Common_Utils_isPOT = R"doc()doc";

static const char *__doc_Common_Utils_padSize = R"doc(Pad an integer to a multiple of another)doc";

static const char *__doc_Common_Utils_popCount = R"doc()doc";

static const char *__doc_Common_Utils_setThreadName = R"doc()doc";

static const char *__doc_Common_Utils_updateHash = R"doc(Hash arithmetic types and enums)doc";

static const char *__doc_Common_Utils_updateHash_2 = R"doc(Hash monostate)doc";

static const char *__doc_Common_Utils_updateHash_3 = R"doc(Hash strings)doc";

static const char *__doc_Common_Utils_updateHash_4 = R"doc(Hash arrays of types which can, themselves, be hashed)doc";

static const char *__doc_Common_Utils_updateHash_5 = R"doc(Hash vectors of types which can, themselves, be hashed)doc";

static const char *__doc_Common_Utils_updateHash_6 = R"doc(Hash vectors of bools)doc";

static const char *__doc_Common_Utils_updateHash_7 = R"doc(Hash unordered maps of types which can, themselves, be hashed)doc";

static const char *__doc_Common_Utils_updateHash_8 = R"doc(Hash unordered sets of types which can, themselves, be hashed)doc";

static const char *__doc_Common_Utils_updateHash_9 = R"doc(Hash optional types which can, themeselves, be hashed)doc";

static const char *__doc_Common_Utils_updateHash_10 = R"doc(Hash variants of types which can, themeselves, be hashed)doc";

static const char *__doc_CompilerFrontend_ErrorHandler = R"doc()doc";

static const char *__doc_CompilerFrontend_ErrorHandlerBase = R"doc()doc";

static const char *__doc_CompilerFrontend_ErrorHandlerBase_2 = R"doc()doc";

static const char *__doc_CompilerFrontend_ErrorHandlerBase_3 = R"doc()doc";

static const char *__doc_CompilerFrontend_ErrorHandlerBase_4 = R"doc()doc";

static const char *__doc_CompilerFrontend_ErrorHandlerBase_error = R"doc()doc";

static const char *__doc_CompilerFrontend_ErrorHandlerBase_error_2 = R"doc()doc";

static const char *__doc_CompilerFrontend_ErrorHandler_ErrorHandler = R"doc()doc";

static const char *__doc_CompilerFrontend_ErrorHandler_error = R"doc()doc";

static const char *__doc_CompilerFrontend_ErrorHandler_error_2 = R"doc()doc";

static const char *__doc_CompilerFrontend_ErrorHandler_getContext = R"doc()doc";

static const char *__doc_CompilerFrontend_ErrorHandler_hasError = R"doc()doc";

static const char *__doc_CompilerFrontend_ErrorHandler_m_Context = R"doc()doc";

static const char *__doc_CompilerFrontend_ErrorHandler_m_Error = R"doc()doc";

static const char *__doc_CompilerFrontend_ErrorHandler_report = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Acceptable = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Acceptable_accept = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_ArraySubscript = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_ArraySubscript_2 = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_ArraySubscript_ArraySubscript = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_ArraySubscript_getArray = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_ArraySubscript_getClosingSquareBracket = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_ArraySubscript_getIndex = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_ArraySubscript_isLValue = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_ArraySubscript_m_Array = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_ArraySubscript_m_ClosingSquareBracket = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_ArraySubscript_m_Index = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Assignment = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Assignment_2 = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Assignment_Assignment = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Assignment_getAssignee = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Assignment_getOperator = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Assignment_getValue = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Assignment_m_Assignee = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Assignment_m_Operator = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Assignment_m_Value = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Base = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Base_accept = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Base_isLValue = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Binary = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Binary_2 = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Binary_Binary = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Binary_getLeft = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Binary_getOperator = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Binary_getRight = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Binary_m_Left = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Binary_m_Operator = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Binary_m_Right = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Call = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Call_2 = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Call_Call = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Call_getArguments = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Call_getCallee = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Call_getClosingParen = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Call_m_Arguments = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Call_m_Callee = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Call_m_ClosingParen = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Cast = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Cast_2 = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Cast_Cast = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Cast_getClosingParen = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Cast_getExpression = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Cast_getType = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Cast_m_ClosingParen = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Cast_m_Expression = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Cast_m_Type = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Conditional = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Conditional_2 = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Conditional_Conditional = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Conditional_getCondition = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Conditional_getFalse = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Conditional_getQuestion = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Conditional_getTrue = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Conditional_m_Condition = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Conditional_m_False = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Conditional_m_Question = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Conditional_m_True = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Grouping = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Grouping_2 = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Grouping_Grouping = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Grouping_getExpression = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Grouping_isLValue = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Grouping_m_Expression = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Identifier = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Identifier_2 = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Identifier_Identifier = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Identifier_getName = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Identifier_isLValue = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Identifier_m_Name = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Literal = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Literal_2 = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Literal_Literal = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Literal_getValue = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Literal_isLValue = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Literal_m_Value = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Logical = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Logical_2 = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Logical_Logical = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Logical_getLeft = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Logical_getOperator = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Logical_getRight = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Logical_m_Left = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Logical_m_Operator = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Logical_m_Right = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_PostfixIncDec = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_PostfixIncDec_2 = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_PostfixIncDec_PostfixIncDec = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_PostfixIncDec_getOperator = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_PostfixIncDec_getTarget = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_PostfixIncDec_m_Operator = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_PostfixIncDec_m_Target = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_PrefixIncDec = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_PrefixIncDec_2 = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_PrefixIncDec_PrefixIncDec = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_PrefixIncDec_getOperator = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_PrefixIncDec_getTarget = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_PrefixIncDec_m_Operator = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_PrefixIncDec_m_Target = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Unary = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Unary_2 = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Unary_Unary = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Unary_getOperator = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Unary_getRight = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Unary_isLValue = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Unary_m_Operator = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Unary_m_Right = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Visitor = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Visitor_visit = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Visitor_visit_2 = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Visitor_visit_3 = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Visitor_visit_4 = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Visitor_visit_5 = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Visitor_visit_6 = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Visitor_visit_7 = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Visitor_visit_8 = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Visitor_visit_9 = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Visitor_visit_10 = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Visitor_visit_11 = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Visitor_visit_12 = R"doc()doc";

static const char *__doc_CompilerFrontend_Expression_Visitor_visit_13 = R"doc()doc";

static const char *__doc_CompilerFrontend_Parser_ParseError = R"doc()doc";

static const char *__doc_CompilerFrontend_Parser_ParseError_ParseError = R"doc()doc";

static const char *__doc_CompilerFrontend_Parser_parseBlockItemList =
R"doc(Parse block item list from tokens
Block item lists are function body scope list of statements)doc";

static const char *__doc_CompilerFrontend_Parser_parseExpression = R"doc(Parse expression from tokens)doc";

static const char *__doc_CompilerFrontend_Parser_parseNumericType = R"doc(Parse type from tokens)doc";

static const char *__doc_CompilerFrontend_PythonType_getPythonTypeString = R"doc()doc";

static const char *__doc_CompilerFrontend_PythonType_log2 = R"doc()doc";

static const char *__doc_CompilerFrontend_Scanner_scanSource = R"doc()doc";

static const char *__doc_CompilerFrontend_SingleLineErrorHandler = R"doc()doc";

static const char *__doc_CompilerFrontend_SingleLineErrorHandler_SingleLineErrorHandler = R"doc()doc";

static const char *__doc_CompilerFrontend_SingleLineErrorHandler_error = R"doc()doc";

static const char *__doc_CompilerFrontend_SingleLineErrorHandler_error_2 = R"doc()doc";

static const char *__doc_CompilerFrontend_SingleLineErrorHandler_hasError = R"doc()doc";

static const char *__doc_CompilerFrontend_SingleLineErrorHandler_m_Error = R"doc()doc";

static const char *__doc_CompilerFrontend_SingleLineErrorHandler_report = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_Acceptable = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_Acceptable_accept = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_Base = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_Base_accept = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_Break = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_Break_2 = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_Break_Break = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_Break_getToken = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_Break_m_Token = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_Compound = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_Compound_2 = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_Compound_Compound = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_Compound_getStatements = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_Compound_m_Statements = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_Continue = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_Continue_2 = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_Continue_Continue = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_Continue_getToken = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_Continue_m_Token = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_Do = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_Do_2 = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_Do_Do = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_Do_getBody = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_Do_getCondition = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_Do_getWhile = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_Do_m_Body = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_Do_m_Condition = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_Do_m_While = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_Expression = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_Expression_2 = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_Expression_Expression = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_Expression_getExpression = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_Expression_m_Expression = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_For = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_For_2 = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_ForEachSynapse = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_ForEachSynapse_2 = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_ForEachSynapse_ForEachSynapse = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_ForEachSynapse_getBody = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_ForEachSynapse_getForEachSynapse = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_ForEachSynapse_m_Body = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_ForEachSynapse_m_ForEachSynapse = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_For_For = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_For_getBody = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_For_getCondition = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_For_getFor = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_For_getIncrement = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_For_getInitialiser = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_For_m_Body = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_For_m_Condition = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_For_m_For = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_For_m_Increment = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_For_m_Initialiser = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_If = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_If_2 = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_If_If = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_If_getCondition = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_If_getElseBranch = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_If_getIf = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_If_getThenBranch = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_If_m_Condition = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_If_m_ElseBranch = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_If_m_If = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_If_m_ThenBranch = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_Labelled = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_Labelled_2 = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_Labelled_Labelled = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_Labelled_getBody = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_Labelled_getKeyword = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_Labelled_getValue = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_Labelled_m_Body = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_Labelled_m_Keyword = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_Labelled_m_Value = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_Switch = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_Switch_2 = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_Switch_Switch = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_Switch_getBody = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_Switch_getCondition = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_Switch_getSwitch = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_Switch_m_Body = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_Switch_m_Condition = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_Switch_m_Switch = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_VarDeclaration = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_VarDeclaration_2 = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_VarDeclaration_VarDeclaration = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_VarDeclaration_getInitDeclaratorList = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_VarDeclaration_getType = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_VarDeclaration_m_DeclarationSpecifiers = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_VarDeclaration_m_InitDeclaratorList = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_VarDeclaration_m_Type = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_Visitor = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_Visitor_visit = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_Visitor_visit_2 = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_Visitor_visit_3 = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_Visitor_visit_4 = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_Visitor_visit_5 = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_Visitor_visit_6 = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_Visitor_visit_7 = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_Visitor_visit_8 = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_Visitor_visit_9 = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_Visitor_visit_10 = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_Visitor_visit_11 = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_Visitor_visit_12 = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_While = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_While_2 = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_While_While = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_While_getBody = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_While_getCondition = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_While_getWhile = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_While_m_Body = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_While_m_Condition = R"doc()doc";

static const char *__doc_CompilerFrontend_Statement_While_m_While = R"doc()doc";

static const char *__doc_CompilerFrontend_Token = R"doc()doc";

static const char *__doc_CompilerFrontend_Token_2 = R"doc()doc";

static const char *__doc_CompilerFrontend_Token_Token = R"doc()doc";

static const char *__doc_CompilerFrontend_Token_Type = R"doc()doc";

static const char *__doc_CompilerFrontend_Token_Type_AMPERSAND = R"doc()doc";

static const char *__doc_CompilerFrontend_Token_Type_AMPERSAND_AMPERSAND = R"doc()doc";

static const char *__doc_CompilerFrontend_Token_Type_AMPERSAND_EQUAL = R"doc()doc";

static const char *__doc_CompilerFrontend_Token_Type_AT = R"doc()doc";

static const char *__doc_CompilerFrontend_Token_Type_BOOLEAN = R"doc()doc";

static const char *__doc_CompilerFrontend_Token_Type_BREAK = R"doc()doc";

static const char *__doc_CompilerFrontend_Token_Type_CARET = R"doc()doc";

static const char *__doc_CompilerFrontend_Token_Type_CARET_EQUAL = R"doc()doc";

static const char *__doc_CompilerFrontend_Token_Type_CASE = R"doc()doc";

static const char *__doc_CompilerFrontend_Token_Type_COLON = R"doc()doc";

static const char *__doc_CompilerFrontend_Token_Type_COMMA = R"doc()doc";

static const char *__doc_CompilerFrontend_Token_Type_CONTINUE = R"doc()doc";

static const char *__doc_CompilerFrontend_Token_Type_DEFAULT = R"doc()doc";

static const char *__doc_CompilerFrontend_Token_Type_DO = R"doc()doc";

static const char *__doc_CompilerFrontend_Token_Type_DOT = R"doc()doc";

static const char *__doc_CompilerFrontend_Token_Type_ELSE = R"doc()doc";

static const char *__doc_CompilerFrontend_Token_Type_END_OF_FILE = R"doc()doc";

static const char *__doc_CompilerFrontend_Token_Type_EQUAL = R"doc()doc";

static const char *__doc_CompilerFrontend_Token_Type_EQUAL_EQUAL = R"doc()doc";

static const char *__doc_CompilerFrontend_Token_Type_FOR = R"doc()doc";

static const char *__doc_CompilerFrontend_Token_Type_FOR_EACH_SYNAPSE = R"doc()doc";

static const char *__doc_CompilerFrontend_Token_Type_GREATER = R"doc()doc";

static const char *__doc_CompilerFrontend_Token_Type_GREATER_EQUAL = R"doc()doc";

static const char *__doc_CompilerFrontend_Token_Type_IDENTIFIER = R"doc()doc";

static const char *__doc_CompilerFrontend_Token_Type_IF = R"doc()doc";

static const char *__doc_CompilerFrontend_Token_Type_LEFT_BRACE = R"doc()doc";

static const char *__doc_CompilerFrontend_Token_Type_LEFT_PAREN = R"doc()doc";

static const char *__doc_CompilerFrontend_Token_Type_LEFT_SQUARE_BRACKET = R"doc()doc";

static const char *__doc_CompilerFrontend_Token_Type_LESS = R"doc()doc";

static const char *__doc_CompilerFrontend_Token_Type_LESS_EQUAL = R"doc()doc";

static const char *__doc_CompilerFrontend_Token_Type_MINUS = R"doc()doc";

static const char *__doc_CompilerFrontend_Token_Type_MINUS_EQUAL = R"doc()doc";

static const char *__doc_CompilerFrontend_Token_Type_MINUS_MINUS = R"doc()doc";

static const char *__doc_CompilerFrontend_Token_Type_NOT = R"doc()doc";

static const char *__doc_CompilerFrontend_Token_Type_NOT_EQUAL = R"doc()doc";

static const char *__doc_CompilerFrontend_Token_Type_NUMBER = R"doc()doc";

static const char *__doc_CompilerFrontend_Token_Type_PERCENT = R"doc()doc";

static const char *__doc_CompilerFrontend_Token_Type_PERCENT_EQUAL = R"doc()doc";

static const char *__doc_CompilerFrontend_Token_Type_PIPE = R"doc()doc";

static const char *__doc_CompilerFrontend_Token_Type_PIPE_EQUAL = R"doc()doc";

static const char *__doc_CompilerFrontend_Token_Type_PIPE_PIPE = R"doc()doc";

static const char *__doc_CompilerFrontend_Token_Type_PLUS = R"doc()doc";

static const char *__doc_CompilerFrontend_Token_Type_PLUS_EQUAL = R"doc()doc";

static const char *__doc_CompilerFrontend_Token_Type_PLUS_PLUS = R"doc()doc";

static const char *__doc_CompilerFrontend_Token_Type_QUESTION = R"doc()doc";

static const char *__doc_CompilerFrontend_Token_Type_RIGHT_BRACE = R"doc()doc";

static const char *__doc_CompilerFrontend_Token_Type_RIGHT_PAREN = R"doc()doc";

static const char *__doc_CompilerFrontend_Token_Type_RIGHT_SQUARE_BRACKET = R"doc()doc";

static const char *__doc_CompilerFrontend_Token_Type_SEMICOLON = R"doc()doc";

static const char *__doc_CompilerFrontend_Token_Type_SHIFT_LEFT = R"doc()doc";

static const char *__doc_CompilerFrontend_Token_Type_SHIFT_LEFT_EQUAL = R"doc()doc";

static const char *__doc_CompilerFrontend_Token_Type_SHIFT_RIGHT = R"doc()doc";

static const char *__doc_CompilerFrontend_Token_Type_SHIFT_RIGHT_EQUAL = R"doc()doc";

static const char *__doc_CompilerFrontend_Token_Type_SLASH = R"doc()doc";

static const char *__doc_CompilerFrontend_Token_Type_SLASH_EQUAL = R"doc()doc";

static const char *__doc_CompilerFrontend_Token_Type_STAR = R"doc()doc";

static const char *__doc_CompilerFrontend_Token_Type_STAR_EQUAL = R"doc()doc";

static const char *__doc_CompilerFrontend_Token_Type_STRING = R"doc()doc";

static const char *__doc_CompilerFrontend_Token_Type_SWITCH = R"doc()doc";

static const char *__doc_CompilerFrontend_Token_Type_TILDA = R"doc()doc";

static const char *__doc_CompilerFrontend_Token_Type_TYPE_QUALIFIER = R"doc()doc";

static const char *__doc_CompilerFrontend_Token_Type_TYPE_SPECIFIER = R"doc()doc";

static const char *__doc_CompilerFrontend_Token_Type_WHILE = R"doc()doc";

static const char *__doc_CompilerFrontend_Token_index = R"doc()doc";

static const char *__doc_CompilerFrontend_Token_lexeme = R"doc()doc";

static const char *__doc_CompilerFrontend_Token_line = R"doc()doc";

static const char *__doc_CompilerFrontend_Token_numberType = R"doc()doc";

static const char *__doc_CompilerFrontend_Token_type = R"doc()doc";

static const char *__doc_CompilerFrontend_TypeChecker_EnvironmentBase = R"doc()doc";

static const char *__doc_CompilerFrontend_TypeChecker_EnvironmentBase_define = R"doc()doc";

static const char *__doc_CompilerFrontend_TypeChecker_EnvironmentBase_getType = R"doc()doc";

static const char *__doc_CompilerFrontend_TypeChecker_EnvironmentBase_getTypes = R"doc()doc";

static const char *__doc_CompilerFrontend_TypeChecker_EnvironmentInternal = R"doc()doc";

static const char *__doc_CompilerFrontend_TypeChecker_EnvironmentInternal_EnvironmentInternal = R"doc()doc";

static const char *__doc_CompilerFrontend_TypeChecker_EnvironmentInternal_define = R"doc()doc";

static const char *__doc_CompilerFrontend_TypeChecker_EnvironmentInternal_getTypes = R"doc()doc";

static const char *__doc_CompilerFrontend_TypeChecker_EnvironmentInternal_m_Enclosing = R"doc()doc";

static const char *__doc_CompilerFrontend_TypeChecker_EnvironmentInternal_m_Types = R"doc()doc";

static const char *__doc_CompilerFrontend_TypeChecker_TypeCheckError = R"doc()doc";

static const char *__doc_CompilerFrontend_TypeChecker_TypeCheckError_TypeCheckError = R"doc()doc";

static const char *__doc_CompilerFrontend_TypeChecker_typeCheck = R"doc()doc";

static const char *__doc_CompilerFrontend_TypeChecker_typeCheck_2 = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_FunctionFlags = R"doc(Flags that can be applied to function types)doc";

static const char *__doc_CompilerFrontend_Type_FunctionFlags_ARRAY_SUBSCRIPT_OVERRIDE = R"doc(Function is variadic)doc";

static const char *__doc_CompilerFrontend_Type_FunctionFlags_VARIADIC = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_NumericValue =
R"doc(ResolvedType::Numeric has various values attached e.g. min and max. These
Cannot be represented using any single type (double can't represent all uint64_t for example)
Therefore, this type is used as a wrapper.)doc";

static const char *__doc_CompilerFrontend_Type_NumericValue_NumericValue = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_NumericValue_NumericValue_2 = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_NumericValue_NumericValue_3 = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_NumericValue_NumericValue_4 = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_NumericValue_NumericValue_5 = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_NumericValue_cast = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_NumericValue_get = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_NumericValue_m_Value = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_NumericValue_operator_eq = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_NumericValue_operator_ge = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_NumericValue_operator_gt = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_NumericValue_operator_le = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_NumericValue_operator_lt = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_NumericValue_operator_ne = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_ResolvedType = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_ResolvedType_2 = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_ResolvedType_Function = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_ResolvedType_Function_Function = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_ResolvedType_Function_Function_2 = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_ResolvedType_Function_argTypes = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_ResolvedType_Function_flags = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_ResolvedType_Function_hasFlag = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_ResolvedType_Function_operator_assign = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_ResolvedType_Function_operator_eq = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_ResolvedType_Function_operator_lt = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_ResolvedType_Function_operator_ne = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_ResolvedType_Function_returnType = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_ResolvedType_Numeric = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_ResolvedType_Numeric_fixedPoint = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_ResolvedType_Numeric_isIntegral = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_ResolvedType_Numeric_isSaturating = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_ResolvedType_Numeric_isSigned = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_ResolvedType_Numeric_literalSuffix = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_ResolvedType_Numeric_lowest = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_ResolvedType_Numeric_max = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_ResolvedType_Numeric_maxDigits10 = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_ResolvedType_Numeric_min = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_ResolvedType_Numeric_operator_eq = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_ResolvedType_Numeric_operator_lt = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_ResolvedType_Numeric_operator_ne = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_ResolvedType_Numeric_pythonFormat = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_ResolvedType_Numeric_rank = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_ResolvedType_Pointer = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_ResolvedType_Pointer_Pointer = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_ResolvedType_Pointer_Pointer_2 = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_ResolvedType_Pointer_operator_assign = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_ResolvedType_Pointer_operator_eq = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_ResolvedType_Pointer_operator_lt = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_ResolvedType_Pointer_operator_ne = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_ResolvedType_Pointer_valueType = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_ResolvedType_ResolvedType = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_ResolvedType_ResolvedType_2 = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_ResolvedType_ResolvedType_3 = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_ResolvedType_ResolvedType_4 = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_ResolvedType_ResolvedType_5 = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_ResolvedType_Value = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_ResolvedType_Value_device = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_ResolvedType_Value_ffiType = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_ResolvedType_Value_isWriteOnly = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_ResolvedType_Value_name = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_ResolvedType_Value_numeric = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_ResolvedType_Value_operator_eq = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_ResolvedType_Value_operator_lt = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_ResolvedType_Value_operator_ne = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_ResolvedType_Value_size = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_ResolvedType_addConst = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_ResolvedType_addWriteOnly = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_ResolvedType_createFixedPointNumeric = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_ResolvedType_createFunction = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_ResolvedType_createNumeric = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_ResolvedType_createPointer = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_ResolvedType_createValue = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_ResolvedType_createValue_2 = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_ResolvedType_detail = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_ResolvedType_getFFIType = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_ResolvedType_getFunction = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_ResolvedType_getName = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_ResolvedType_getNumeric = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_ResolvedType_getPointer = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_ResolvedType_getSize = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_ResolvedType_getValue = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_ResolvedType_isConst = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_ResolvedType_isFunction = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_ResolvedType_isNumeric = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_ResolvedType_isPointer = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_ResolvedType_isPointerToPointer = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_ResolvedType_isScalar = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_ResolvedType_isValue = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_ResolvedType_isVoid = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_ResolvedType_operator_eq = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_ResolvedType_operator_lt = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_ResolvedType_operator_ne = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_ResolvedType_removeConst = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_UnresolvedType = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_UnresolvedType_UnresolvedType = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_UnresolvedType_UnresolvedType_2 = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_UnresolvedType_detail = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_UnresolvedType_operator_eq = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_UnresolvedType_operator_lt = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_UnresolvedType_operator_ne = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_UnresolvedType_resolve = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_getAddToPrePost = R"doc(Get type to add a weight type)doc";

static const char *__doc_CompilerFrontend_Type_getAddToPrePostDelay = R"doc(Get type to add a weight type with delay)doc";

static const char *__doc_CompilerFrontend_Type_getArraySubscript = R"doc(Get type for array subscript overload functions)doc";

static const char *__doc_CompilerFrontend_Type_getCommonType = R"doc(Apply C rules to get common type between numeric types a and b)doc";

static const char *__doc_CompilerFrontend_Type_getPromotedType = R"doc(Apply C type promotion rules to numeric type)doc";

static const char *__doc_CompilerFrontend_Type_operator_band = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_operator_bor = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_serialiseNumeric = R"doc(Serialise numeric value to bytes)doc";

static const char *__doc_CompilerFrontend_Type_updateHash = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_updateHash_2 = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_updateHash_3 = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_updateHash_4 = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_updateHash_5 = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_updateHash_6 = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_updateHash_7 = R"doc()doc";

static const char *__doc_CompilerFrontend_Type_writeNumeric = R"doc(Write numeric value to string, formatting correctly for type)doc";

static const char *__doc_Frontend_ArrayBase = R"doc()doc";

static const char *__doc_Frontend_ArrayBase_2 =
R"doc(Base class for arrays created by backend
**NOTE** this is a temporary, simplified version of GeNN's ArrayBase)doc";

static const char *__doc_Frontend_ArrayBase_ArrayBase = R"doc()doc";

static const char *__doc_Frontend_ArrayBase_getHostPointer = R"doc(Get array host pointer)doc";

static const char *__doc_Frontend_ArrayBase_getHostPointer_2 = R"doc()doc";

static const char *__doc_Frontend_ArrayBase_getShape = R"doc()doc";

static const char *__doc_Frontend_ArrayBase_getSizeBytes = R"doc()doc";

static const char *__doc_Frontend_ArrayBase_getStrides = R"doc()doc";

static const char *__doc_Frontend_ArrayBase_getType = R"doc()doc";

static const char *__doc_Frontend_ArrayBase_m_HostPointer = R"doc()doc";

static const char *__doc_Frontend_ArrayBase_m_Shape = R"doc()doc";

static const char *__doc_Frontend_ArrayBase_m_SizeBytes = R"doc()doc";

static const char *__doc_Frontend_ArrayBase_m_Strides = R"doc()doc";

static const char *__doc_Frontend_ArrayBase_m_Type = R"doc()doc";

static const char *__doc_Frontend_ArrayBase_memsetHostPointer = R"doc(Memset the host pointer)doc";

static const char *__doc_Frontend_ArrayBase_pullFromDevice = R"doc(Copy entire array from device)doc";

static const char *__doc_Frontend_ArrayBase_pushToDevice = R"doc(Copy entire array to device)doc";

static const char *__doc_Frontend_ArrayBase_serialiseDeviceObject = R"doc(Serialise backend-specific device object to uint32_t)doc";

static const char *__doc_Frontend_ArrayBase_setHostPointer = R"doc()doc";

static const char *__doc_Frontend_DeviceBase = R"doc()doc";

static const char *__doc_Frontend_DeviceBase_2 = R"doc(Interface Runtime classes use to communicate with state on individual devices)doc";

static const char *__doc_Frontend_DeviceBase_DeviceBase = R"doc()doc";

static const char *__doc_Frontend_DeviceBase_DeviceBase_2 = R"doc()doc";

static const char *__doc_Frontend_DeviceBase_DeviceBase_3 = R"doc()doc";

static const char *__doc_Frontend_DeviceBase_createArray = R"doc(Create array to provide storage for model state)doc";

static const char *__doc_Frontend_DeviceBase_createPerformanceCounter = R"doc(Create suitable array for performance counter on this device)doc";

static const char *__doc_Frontend_DeviceBase_getArray = R"doc(Get array associated with model state)doc";

static const char *__doc_Frontend_DeviceBase_getDeviceIndex = R"doc()doc";

static const char *__doc_Frontend_DeviceBase_loadKernel = R"doc(Load kernel onto device)doc";

static const char *__doc_Frontend_DeviceBase_m_Arrays = R"doc()doc";

static const char *__doc_Frontend_DeviceBase_m_DeviceIndex = R"doc()doc";

static const char *__doc_Frontend_DeviceBase_runCurrentKernel = R"doc(Run current kernel on device)doc";

static const char *__doc_Frontend_EventChannel = R"doc()doc";

static const char *__doc_Frontend_EventChannelSink = R"doc(Event sink at the input end of an event channel)doc";

static const char *__doc_Frontend_EventChannelSink_EventChannelSink = R"doc()doc";

static const char *__doc_Frontend_EventChannelSink_createArray = R"doc()doc";

static const char *__doc_Frontend_EventChannelSink_getArrayShapeStride = R"doc()doc";

static const char *__doc_Frontend_EventChannelSink_getShape = R"doc()doc";

static const char *__doc_Frontend_EventChannelSink_getSource = R"doc()doc";

static const char *__doc_Frontend_EventChannelSink_m_Record = R"doc()doc";

static const char *__doc_Frontend_EventChannelSink_m_Shape = R"doc()doc";

static const char *__doc_Frontend_EventChannelSink_m_Source = R"doc()doc";

static const char *__doc_Frontend_EventChannelSink_shouldRecord = R"doc()doc";

static const char *__doc_Frontend_EventChannelSink_updateMergeHash = R"doc()doc";

static const char *__doc_Frontend_EventChannelSource = R"doc(Event source at the output end of an event channel)doc";

static const char *__doc_Frontend_EventChannelSource_EventChannelSource = R"doc()doc";

static const char *__doc_Frontend_EventChannelSource_createArray = R"doc()doc";

static const char *__doc_Frontend_EventChannelSource_getArrayShapeStride = R"doc()doc";

static const char *__doc_Frontend_EventChannelSource_getShape = R"doc()doc";

static const char *__doc_Frontend_EventChannelSource_m_Shape = R"doc()doc";

static const char *__doc_Frontend_EventChannelSource_updateMergeHash = R"doc()doc";

static const char *__doc_Frontend_EventChannel_EventChannel = R"doc()doc";

static const char *__doc_Frontend_EventChannel_create = R"doc()doc";

static const char *__doc_Frontend_EventChannel_getSink = R"doc()doc";

static const char *__doc_Frontend_EventChannel_getSource = R"doc()doc";

static const char *__doc_Frontend_EventChannel_m_Sink = R"doc()doc";

static const char *__doc_Frontend_EventChannel_m_Source = R"doc()doc";

static const char *__doc_Frontend_EventPropagationProcess = R"doc()doc";

static const char *__doc_Frontend_EventPropagationProcess_EventPropagationProcess = R"doc()doc";

static const char *__doc_Frontend_EventPropagationProcess_create = R"doc()doc";

static const char *__doc_Frontend_EventPropagationProcess_getAllEventSinks = R"doc(Get vector of event sink objects used by this process)doc";

static const char *__doc_Frontend_EventPropagationProcess_getAllEventSources = R"doc(Get vector of event source objects used by this process)doc";

static const char *__doc_Frontend_EventPropagationProcess_getAllState = R"doc(Get vector of state objects used by this process)doc";

static const char *__doc_Frontend_EventPropagationProcess_getInputEventSource = R"doc()doc";

static const char *__doc_Frontend_EventPropagationProcess_getTarget = R"doc()doc";

static const char *__doc_Frontend_EventPropagationProcess_m_InputEventSource = R"doc()doc";

static const char *__doc_Frontend_EventPropagationProcess_m_Target = R"doc()doc";

static const char *__doc_Frontend_EventPropagationProcess_updateCompatibleSplitDimensions =
R"doc(Update the compatible split dimensions of a state object (which should be
one used by this process) with any constraints imposed by this process))doc";

static const char *__doc_Frontend_EventPropagationProcess_updateMergeHash = R"doc(Update the provided hash with the properties of this process which determine whether it can be merged)doc";

static const char *__doc_Frontend_EventSink =
R"doc(Event sinks take events from model components
which emit them e.g. NeuronUpdateProcess)doc";

static const char *__doc_Frontend_EventSinkBuffer = R"doc(A buffer for recording events emitted from a process)doc";

static const char *__doc_Frontend_EventSinkBuffer_EventSinkBuffer = R"doc()doc";

static const char *__doc_Frontend_EventSinkBuffer_create = R"doc()doc";

static const char *__doc_Frontend_EventSinkBuffer_createArray = R"doc()doc";

static const char *__doc_Frontend_EventSinkBuffer_getArrayShapeStride = R"doc()doc";

static const char *__doc_Frontend_EventSinkBuffer_getShape = R"doc()doc";

static const char *__doc_Frontend_EventSinkBuffer_m_Shape = R"doc()doc";

static const char *__doc_Frontend_EventSinkBuffer_updateMergeHash = R"doc()doc";

static const char *__doc_Frontend_EventSource =
R"doc(Event sources produce events for model components
which require them e.g. EventPropagationProcess)doc";

static const char *__doc_Frontend_EventSourceBuffer = R"doc(A buffer which provides a source of events)doc";

static const char *__doc_Frontend_EventSourceBuffer_EventSourceBuffer = R"doc()doc";

static const char *__doc_Frontend_EventSourceBuffer_create = R"doc()doc";

static const char *__doc_Frontend_EventSourceBuffer_createArray = R"doc()doc";

static const char *__doc_Frontend_EventSourceBuffer_getArrayShapeStride = R"doc()doc";

static const char *__doc_Frontend_EventSourceBuffer_getMaxEvents = R"doc()doc";

static const char *__doc_Frontend_EventSourceBuffer_getShape = R"doc()doc";

static const char *__doc_Frontend_EventSourceBuffer_m_MaxEvents = R"doc()doc";

static const char *__doc_Frontend_EventSourceBuffer_m_Shape = R"doc()doc";

static const char *__doc_Frontend_EventSourceBuffer_updateMergeHash = R"doc()doc";

static const char *__doc_Frontend_Kernel = R"doc()doc";

static const char *__doc_Frontend_Kernel_2 = R"doc()doc";

static const char *__doc_Frontend_Kernel_3 = R"doc()doc";

static const char *__doc_Frontend_Kernel_getAllProcessGroups = R"doc(Get all process groups in graph)doc";

static const char *__doc_Frontend_MemsetProcess = R"doc(Process for memsetting variables)doc";

static const char *__doc_Frontend_MemsetProcess_MemsetProcess = R"doc()doc";

static const char *__doc_Frontend_MemsetProcess_create = R"doc()doc";

static const char *__doc_Frontend_MemsetProcess_getAllEventSinks = R"doc(Get vector of event sink objects used by this process)doc";

static const char *__doc_Frontend_MemsetProcess_getAllEventSources = R"doc(Get vector of event source objects used by this process)doc";

static const char *__doc_Frontend_MemsetProcess_getAllState = R"doc(Get vector of state objects used by this process)doc";

static const char *__doc_Frontend_MemsetProcess_getTarget = R"doc()doc";

static const char *__doc_Frontend_MemsetProcess_m_Target = R"doc()doc";

static const char *__doc_Frontend_MemsetProcess_updateCompatibleSplitDimensions =
R"doc(Update the compatible split dimensions of a state object (which should be
one used by this process) with any constraints imposed by this process))doc";

static const char *__doc_Frontend_MemsetProcess_updateMergeHash = R"doc(Update the provided hash with the properties of this process which determine whether it can be merged)doc";

static const char *__doc_Frontend_Merged = R"doc()doc";

static const char *__doc_Frontend_MergedProcessGroup = R"doc()doc";

static const char *__doc_Frontend_MergedProcessGroup_MergedProcessGroup = R"doc()doc";

static const char *__doc_Frontend_MergedProcessGroup_getDestination = R"doc(Determine which merged process a process has ended up in following merging)doc";

static const char *__doc_Frontend_MergedProcessGroup_getMergedProcesses = R"doc(Get vector of merged processes within this groups)doc";

static const char *__doc_Frontend_MergedProcessGroup_m_Destinations = R"doc()doc";

static const char *__doc_Frontend_MergedProcessGroup_m_MergedProcesses = R"doc()doc";

static const char *__doc_Frontend_Merged_Merged = R"doc()doc";

static const char *__doc_Frontend_Merged_forEachMerged = R"doc()doc";

static const char *__doc_Frontend_Merged_getArchetype = R"doc(Get 'archetype' process - it's properties represent those of all other merged processes)doc";

static const char *__doc_Frontend_Merged_getArchetype_2 = R"doc(Get 'archetype' process - it's properties represent those of all other merged processes)doc";

static const char *__doc_Frontend_Merged_getIndex = R"doc()doc";

static const char *__doc_Frontend_Merged_getMerged = R"doc(Get vector of merged processes)doc";

static const char *__doc_Frontend_Merged_m_Index = R"doc()doc";

static const char *__doc_Frontend_Merged_m_Merged = R"doc()doc";

static const char *__doc_Frontend_Model =
R"doc(A model encompasses of a number of graphs which share the same state.
It also contains any additional data structures required BEFORE merging.
If a backend requires additional data to calculate mergibility,
it needs to be added to a class derived from Model)doc";

static const char *__doc_Frontend_Model_2 =
R"doc(A model encompasses of a number of graphs which share the same state.
It also contains any additional data structures required BEFORE merging.
If a backend requires additional data to calculate mergibility,
it needs to be added to a class derived from Model)doc";

static const char *__doc_Frontend_Model_3 = R"doc()doc";

static const char *__doc_Frontend_Model_4 = R"doc()doc";

static const char *__doc_Frontend_Model_5 = R"doc()doc";

static const char *__doc_Frontend_ModelComponent = R"doc(Base class for things that can be added to models)doc";

static const char *__doc_Frontend_ModelComponent_ModelComponent = R"doc()doc";

static const char *__doc_Frontend_ModelComponent_ModelComponent_2 = R"doc()doc";

static const char *__doc_Frontend_ModelComponent_Private = R"doc()doc";

static const char *__doc_Frontend_ModelComponent_Private_Private = R"doc()doc";

static const char *__doc_Frontend_ModelComponent_getName = R"doc()doc";

static const char *__doc_Frontend_ModelComponent_m_Name = R"doc()doc";

static const char *__doc_Frontend_Model_Model = R"doc()doc";

static const char *__doc_Frontend_Model_StateData = R"doc()doc";

static const char *__doc_Frontend_Model_StateData_indexDimensions = R"doc()doc";

static const char *__doc_Frontend_Model_StateData_processes = R"doc()doc";

static const char *__doc_Frontend_Model_StateData_splitDimension = R"doc()doc";

static const char *__doc_Frontend_Model_getKernels = R"doc()doc";

static const char *__doc_Frontend_Model_getStateData = R"doc()doc";

static const char *__doc_Frontend_Model_getStateData_2 = R"doc()doc";

static const char *__doc_Frontend_Model_m_Kernels = R"doc()doc";

static const char *__doc_Frontend_Model_m_StateData = R"doc()doc";

static const char *__doc_Frontend_NeuronUpdateProcess = R"doc()doc";

static const char *__doc_Frontend_NeuronUpdateProcess_NeuronUpdateProcess = R"doc()doc";

static const char *__doc_Frontend_NeuronUpdateProcess_constrainSplitDimensions =
R"doc(Once compatible split dimensions have been obtained for each state object, they might need
'constraining' e.g. to ensure all variables associated with a neuron update process get split the same)doc";

static const char *__doc_Frontend_NeuronUpdateProcess_create = R"doc()doc";

static const char *__doc_Frontend_NeuronUpdateProcess_getAllEventSinks = R"doc(Get vector of event sink objects used by this process)doc";

static const char *__doc_Frontend_NeuronUpdateProcess_getAllEventSources = R"doc(Get vector of event source objects used by this process)doc";

static const char *__doc_Frontend_NeuronUpdateProcess_getAllState = R"doc(Get vector of state objects used by this process)doc";

static const char *__doc_Frontend_NeuronUpdateProcess_getLiterals = R"doc()doc";

static const char *__doc_Frontend_NeuronUpdateProcess_getOutputEventSinks = R"doc()doc";

static const char *__doc_Frontend_NeuronUpdateProcess_getShape = R"doc()doc";

static const char *__doc_Frontend_NeuronUpdateProcess_getTokens = R"doc()doc";

static const char *__doc_Frontend_NeuronUpdateProcess_getVariables = R"doc()doc";

static const char *__doc_Frontend_NeuronUpdateProcess_m_Literals = R"doc(Vector of literal types and numeric values built from code)doc";

static const char *__doc_Frontend_NeuronUpdateProcess_m_OutputEventSinks = R"doc()doc";

static const char *__doc_Frontend_NeuronUpdateProcess_m_Shape = R"doc()doc";

static const char *__doc_Frontend_NeuronUpdateProcess_m_Tokens = R"doc(Tokens built from code)doc";

static const char *__doc_Frontend_NeuronUpdateProcess_m_Variables = R"doc()doc";

static const char *__doc_Frontend_NeuronUpdateProcess_updateCompatibleSplitDimensions =
R"doc(Update the compatible split dimensions of a state object (which should be
one used by this process) with any constraints imposed by this process))doc";

static const char *__doc_Frontend_NeuronUpdateProcess_updateMergeHash = R"doc(Update the provided hash with the properties of this process which determine whether it can be merged)doc";

static const char *__doc_Frontend_Padding = R"doc()doc";

static const char *__doc_Frontend_Parameter = R"doc()doc";

static const char *__doc_Frontend_Process = R"doc()doc";

static const char *__doc_Frontend_Process_2 = R"doc()doc";

static const char *__doc_Frontend_Process_3 = R"doc()doc";

static const char *__doc_Frontend_Process_4 = R"doc()doc";

static const char *__doc_Frontend_ProcessGroup = R"doc()doc";

static const char *__doc_Frontend_ProcessGroup_2 = R"doc()doc";

static const char *__doc_Frontend_ProcessGroup_3 = R"doc()doc";

static const char *__doc_Frontend_ProcessGroup_4 = R"doc()doc";

static const char *__doc_Frontend_ProcessGroup_ProcessGroup = R"doc()doc";

static const char *__doc_Frontend_ProcessGroup_create = R"doc()doc";

static const char *__doc_Frontend_ProcessGroup_getProcesses = R"doc()doc";

static const char *__doc_Frontend_ProcessGroup_m_Processes = R"doc()doc";

static const char *__doc_Frontend_ProcessGroup_m_RecordPerformance = R"doc()doc";

static const char *__doc_Frontend_ProcessGroup_shouldRecordPerformance = R"doc()doc";

static const char *__doc_Frontend_Process_constrainSplitDimensions =
R"doc(Once compatible split dimensions have been obtained for each state object, they might need
'constraining' e.g. to ensure all variables associated with a neuron update process get split the same)doc";

static const char *__doc_Frontend_Process_getAllEventSinks = R"doc(Get vector of event sink objects used by this process)doc";

static const char *__doc_Frontend_Process_getAllEventSources = R"doc(Get vector of event source objects used by this process)doc";

static const char *__doc_Frontend_Process_getAllState = R"doc(Get vector of state objects used by this process)doc";

static const char *__doc_Frontend_Process_updateCompatibleSplitDimensions =
R"doc(Update the compatible split dimensions of a state object (which should be
one used by this process) with any constraints imposed by this process))doc";

static const char *__doc_Frontend_Process_updateMergeHash = R"doc(Update the provided hash with the properties of this process which determine whether it can be merged)doc";

static const char *__doc_Frontend_RNGInitProcess = R"doc()doc";

static const char *__doc_Frontend_RNGInitProcess_RNGInitProcess = R"doc()doc";

static const char *__doc_Frontend_RNGInitProcess_create = R"doc()doc";

static const char *__doc_Frontend_RNGInitProcess_getAllEventSinks = R"doc(Get vector of event sink objects used by this process)doc";

static const char *__doc_Frontend_RNGInitProcess_getAllEventSources = R"doc(Get vector of event source objects used by this process)doc";

static const char *__doc_Frontend_RNGInitProcess_getAllState = R"doc(Get vector of state objects used by this process)doc";

static const char *__doc_Frontend_RNGInitProcess_getSeed = R"doc()doc";

static const char *__doc_Frontend_RNGInitProcess_m_Seed = R"doc()doc";

static const char *__doc_Frontend_RNGInitProcess_updateCompatibleSplitDimensions =
R"doc(Update the compatible split dimensions of a state object (which should be
one used by this process) with any constraints imposed by this process))doc";

static const char *__doc_Frontend_RNGInitProcess_updateMergeHash = R"doc(Update the provided hash with the properties of this process which determine whether it can be merged)doc";

static const char *__doc_Frontend_Runtime = R"doc()doc";

static const char *__doc_Frontend_Runtime_Command = R"doc(Base class for commands to run on worker threads)doc";

static const char *__doc_Frontend_Runtime_Command_execute = R"doc()doc";

static const char *__doc_Frontend_Runtime_LoadKernelCommand = R"doc(Command for loading kernels onto devices)doc";

static const char *__doc_Frontend_Runtime_LoadKernelCommand_LoadKernelCommand = R"doc()doc";

static const char *__doc_Frontend_Runtime_LoadKernelCommand_execute = R"doc()doc";

static const char *__doc_Frontend_Runtime_LoadKernelCommand_m_Kernel = R"doc()doc";

static const char *__doc_Frontend_Runtime_PullStateCommand = R"doc(Command for pushing)doc";

static const char *__doc_Frontend_Runtime_PullStateCommand_PullStateCommand = R"doc()doc";

static const char *__doc_Frontend_Runtime_PullStateCommand_execute = R"doc()doc";

static const char *__doc_Frontend_Runtime_PullStateCommand_m_State = R"doc()doc";

static const char *__doc_Frontend_Runtime_PushStateCommand = R"doc(Command for pushing)doc";

static const char *__doc_Frontend_Runtime_PushStateCommand_PushStateCommand = R"doc()doc";

static const char *__doc_Frontend_Runtime_PushStateCommand_execute = R"doc()doc";

static const char *__doc_Frontend_Runtime_PushStateCommand_m_State = R"doc()doc";

static const char *__doc_Frontend_Runtime_RunCurrentKernelCommand = R"doc(Command for running kernel on all devices)doc";

static const char *__doc_Frontend_Runtime_RunCurrentKernelCommand_execute = R"doc()doc";

static const char *__doc_Frontend_Runtime_Runtime = R"doc()doc";

static const char *__doc_Frontend_Runtime_allocate = R"doc(Allocate memory for model on device)doc";

static const char *__doc_Frontend_Runtime_allocatePostamble = R"doc(Backend-specific logic to run at end of allocate function)doc";

static const char *__doc_Frontend_Runtime_allocatePreamble = R"doc(Backend-specific logic to run at beginning of allocate function)doc";

static const char *__doc_Frontend_Runtime_createDevice = R"doc(Create suitable device)doc";

static const char *__doc_Frontend_Runtime_getArrays = R"doc(Get array(s) associated with state)doc";

static const char *__doc_Frontend_Runtime_getDeviceArrayShapeStrides = R"doc(Get size and shape of array state will map to on given device)doc";

static const char *__doc_Frontend_Runtime_getDevices = R"doc()doc";

static const char *__doc_Frontend_Runtime_getDevices_2 = R"doc()doc";

static const char *__doc_Frontend_Runtime_getMergedProcessGroups = R"doc(Get map of process groups to merged processes)doc";

static const char *__doc_Frontend_Runtime_getModel = R"doc()doc";

static const char *__doc_Frontend_Runtime_getNumDevices = R"doc()doc";

static const char *__doc_Frontend_Runtime_m_Barrier = R"doc()doc";

static const char *__doc_Frontend_Runtime_m_Command = R"doc()doc";

static const char *__doc_Frontend_Runtime_m_CurrentKernel = R"doc(Current kernel loaded onto all devices)doc";

static const char *__doc_Frontend_Runtime_m_Devices = R"doc()doc";

static const char *__doc_Frontend_Runtime_m_MergedProcessGroups = R"doc()doc";

static const char *__doc_Frontend_Runtime_m_Model = R"doc()doc";

static const char *__doc_Frontend_Runtime_m_NumDevices = R"doc()doc";

static const char *__doc_Frontend_Runtime_m_WorkerRun = R"doc()doc";

static const char *__doc_Frontend_Runtime_m_WorkerThreads = R"doc()doc";

static const char *__doc_Frontend_Runtime_pullStateFromDevice = R"doc(Pull state from all devices)doc";

static const char *__doc_Frontend_Runtime_pushStateToDevice = R"doc(Push state to all devices)doc";

static const char *__doc_Frontend_Runtime_run = R"doc(Run kernel on device)doc";

static const char *__doc_Frontend_Runtime_runCommand = R"doc(Run command on all worker threads)doc";

static const char *__doc_Frontend_Runtime_threadFunction = R"doc(Thread function run on each worker thread to execute commands on device)doc";

static const char *__doc_Frontend_Shape_getStride = R"doc()doc";

static const char *__doc_Frontend_Shape_isScalar = R"doc()doc";

static const char *__doc_Frontend_Shape_toString = R"doc()doc";

static const char *__doc_Frontend_SimpleKernel = R"doc()doc";

static const char *__doc_Frontend_SimpleKernel_SimpleKernel = R"doc()doc";

static const char *__doc_Frontend_SimpleKernel_create = R"doc()doc";

static const char *__doc_Frontend_SimpleKernel_getAllProcessGroups = R"doc(Get all process groups in graph)doc";

static const char *__doc_Frontend_SimpleKernel_getProcessGroups = R"doc()doc";

static const char *__doc_Frontend_SimpleKernel_m_ProcessGroups = R"doc()doc";

static const char *__doc_Frontend_SimulationLoopKernel = R"doc()doc";

static const char *__doc_Frontend_SimulationLoopKernel_SimulationLoopKernel = R"doc()doc";

static const char *__doc_Frontend_SimulationLoopKernel_create = R"doc()doc";

static const char *__doc_Frontend_SimulationLoopKernel_getAllProcessGroups = R"doc(Get all process groups in graph)doc";

static const char *__doc_Frontend_SimulationLoopKernel_getBeginProcessGroups = R"doc()doc";

static const char *__doc_Frontend_SimulationLoopKernel_getEndProcessGroups = R"doc()doc";

static const char *__doc_Frontend_SimulationLoopKernel_getNumTimesteps = R"doc()doc";

static const char *__doc_Frontend_SimulationLoopKernel_getTimestepProcessGroups = R"doc()doc";

static const char *__doc_Frontend_SimulationLoopKernel_m_BeginProcessGroups = R"doc()doc";

static const char *__doc_Frontend_SimulationLoopKernel_m_EndProcessGroups = R"doc()doc";

static const char *__doc_Frontend_SimulationLoopKernel_m_NumTimesteps = R"doc()doc";

static const char *__doc_Frontend_SimulationLoopKernel_m_TimestepProcessGroups = R"doc()doc";

static const char *__doc_Frontend_Sliced = R"doc()doc";

static const char *__doc_Frontend_Sliced_Sliced = R"doc()doc";

static const char *__doc_Frontend_Sliced_getNumTimesteps = R"doc()doc";

static const char *__doc_Frontend_Sliced_getShape = R"doc()doc";

static const char *__doc_Frontend_Sliced_getUnderlying = R"doc()doc";

static const char *__doc_Frontend_Sliced_hasTime = R"doc()doc";

static const char *__doc_Frontend_Sliced_hasTimeSlice = R"doc()doc";

static const char *__doc_Frontend_Sliced_m_Shape = R"doc()doc";

static const char *__doc_Frontend_Sliced_m_TimeSlice = R"doc()doc";

static const char *__doc_Frontend_Sliced_m_Underlying = R"doc()doc";

static const char *__doc_Frontend_Sliced_updateMergeHash = R"doc()doc";

static const char *__doc_Frontend_State = R"doc()doc";

static const char *__doc_Frontend_State_2 = R"doc()doc";

static const char *__doc_Frontend_State_3 = R"doc()doc";

static const char *__doc_Frontend_State_createArray = R"doc()doc";

static const char *__doc_Frontend_State_getArrayShapeStride = R"doc()doc";

static const char *__doc_Frontend_State_getShape = R"doc()doc";

static const char *__doc_Frontend_State_updateMergeHash = R"doc()doc";

static const char *__doc_Frontend_Variable = R"doc()doc";

static const char *__doc_Frontend_Variable_Variable = R"doc()doc";

static const char *__doc_Frontend_Variable_create = R"doc()doc";

static const char *__doc_Frontend_Variable_createArray = R"doc()doc";

static const char *__doc_Frontend_Variable_getArrayShapeStride = R"doc()doc";

static const char *__doc_Frontend_Variable_getShape = R"doc()doc";

static const char *__doc_Frontend_Variable_getType = R"doc()doc";

static const char *__doc_Frontend_Variable_m_Shape = R"doc()doc";

static const char *__doc_Frontend_Variable_m_Type = R"doc()doc";

static const char *__doc_Frontend_Variable_updateMergeHash = R"doc()doc";

static const char *__doc_boost_uuids_detail_sha1 = R"doc()doc";

static const char *__doc_plog_IAppender = R"doc()doc";

#if defined(__GNUG__)
#pragma GCC diagnostic pop
#endif

