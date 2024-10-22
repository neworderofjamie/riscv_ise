from string import digits

def get_clean_name(name):
    # Strip leading and trailing spaces
    name = name.rstrip().lstrip()
    
    # Replace anything that isn't alphanumeric or an underscore with an underscore
    name = "".join(c if c.isalnum() or c == "_" else "_" for c in name)
    
    # Remove any leading digits to get valid name
    return name.lstrip(digits)

def evaluate(code):
    code = code.replace("/", "//")
    return int(eval(code, None, {"XLEN": 32}))