import os

# Get the BODY data from standard input
file = os.environ.get("BODY")

# Read data from the file
body_data = file.read()

# Get the path where you want to create the new file (replace with your actual path)
file_path = os.environ.get("PATH_TRANSLATED")

# Check if the BODY data and PATH_TRANSLATED exist
if body_data and file_path:
    try:
        # Open the file for writing
        with open(file_path, "w") as file:
            # Write the BODY data to the file
            file.write(body_data)
        print("Data written to the file successfully.")
    except Exception as e:
        print("Error:", e)
else:
    print("BODY or PATH_TRANSLATED environment variables are not set.")
