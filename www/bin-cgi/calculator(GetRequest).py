import os

# Define the calculate function first
def calculate(v1, v2, op):
    result = None

    if op == "%2B":
        result = v1 + v2
    elif op == "-":
        result = v1 - v2
    elif op == "*":
        result = v1 * v2
    elif op == "%2F":
        if v2 != 0:
            result = v1 / v2
        else:
            result = "Division by zero"
    else:
        result = "Invalid operator"

    return result

# Get the values of environment variables as integers
v1 = int(os.environ.get("v1", 1))
v2 = int(os.environ.get("v2", 1))
operator = os.environ.get("operator", "+")  # Get the operator from the environment variable
res = calculate(v1, v2, operator)  # Perform the calculation

#sand data about v1 and v2 and operator to server

# Generate the HTML page
# ...

# Generate the HTML page
html_content = """
<!DOCTYPE html>
<html>
<head>
    <title>Calculator</title>
    <style>
        body {{
            font-family: Arial, sans-serif;
            text-align: center;
            background-color: #3AAFA9;
        }}

        .calculator {{
            width: 500px;
            margin: 0 auto;
            padding: 10px;
            border-radius: 15px;
            border: #2b7a78 1px solid;
            background-color: #def2f1;
        }}

        .calculator h1 {{
            font-size: 24px;
            margin: 10px 0;
        }}

        .calculator p {{
            margin: 5px 0;
        }}

        .calculator input[type="text"] {{
            width: 92%;
            padding: 8px;
            margin: 5px 0;
            border-radius: 10px;
            font-size: 1.5rem;
            border: none !important;
        }}

        .calculator button {{
            width: 30%;
            padding: 10px;
            margin: 5px;
            font-size: 18px;
            cursor: pointer;
            border-radius: 10px;
            border: #2b7a78 1px solid;
            background-color: #b7e1e0;
            
        }}

        #result {{
            font-size: 20px;
            margin: 10px 0;
        }}
    </style>
</head>
<body>
    <div class="calculator">
        <h1>Calculator</h1>
        <form action="/bin-cgi/calculator(GetRequest).py" method="get">
            <p>Value 1: <input type="text" name="v1" value="{v1}"></p>
            <p>Value 2: <input type="text" name="v2" value="{v2}"></p>
            <button type="submit" name="operator" value="+">+</button>
            <button type="submit" name="operator" value="-">-</button>
            <button type="submit" name="operator" value="*">*</button>
            <button type="submit" name="operator" value="/">/</button>
        </form>
        <p id="result">Result: {result}</p>
        <div class="row">
    <!-- Add a button to move to the main page -->
    <a href="/"><button>Main Page</button></a>
  </div>
    </div>
</body>
</html>
""".format(v1=v1, v2=v2, operator=operator, result=res)

# Print the HTML content to the standard output
print(html_content)
