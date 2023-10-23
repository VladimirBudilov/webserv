import os

var = os.environ.get("BODY", "1")

if var:
    try:
        result = int(var) + 1
        print(result)
    except ValueError:
        print("Invalid input: BODY is not a valid integer.")
else:
    print("BODY is not set or is an empty string.")