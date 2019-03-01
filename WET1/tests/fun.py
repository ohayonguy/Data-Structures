def sum(num1, num2):
    return num1+num2;

def var_func(condition):
    assert type(condition) == str
    if condition == "sum":
        return sum
    else:
        def mul(num1, num2):
            return num1*num2
        return mul


print(var_func(5)(1,2))