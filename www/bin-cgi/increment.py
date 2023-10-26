import os

file_path = os.environ.get("BODY_FILE")

if os.path.exists(file_path):
    try:
        # Open the file for reading
        with open(file_path, "r") as file:
            # Read the content of the file
            file_data = file.read()
        if file_data:
            try:
                result = int(file_data) + 1
                print("HTTP/1.1 200 OK")
                print("\r\n")
                print(result)
                print("\r\n")
            except ValueError:
                print("Invalid input: BODY is not a valid integer.")
        else:
            print("BODY is not set or is an empty string.")
    except Exception as e:
        print("Error:", e)
