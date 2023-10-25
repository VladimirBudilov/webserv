import os
from datetime import datetime


# Get the file path
file_path = os.environ.get("BODY_FILE")
file_name = "new_file"
file_type = "txt"
# Check if the file exists
if os.path.exists(file_path):
    try:
        # Open the file for reading
        with open(file_path, "r") as file:
            # Read the content of the file
            file_data = file.read()

        # Check if data was read successfully
        if file_data:
            # Split the file_data into words using space as a delimiter
            words = file_data.split()

            # Use the first word to create the new file name
            if words:
                # Get the current time
                current_time = datetime.now()
                # Format the current time as a string with a specific format
                time_format = "%Y%m%d%H%M%S"  # You can customize the format as needed
                formatted_time = current_time.strftime(time_format)
                # Create a new file name with the current time at the beginning
                new_file_name = "tm.txt"
                # Get the path where you want to create the new file
                new_file_path = os.environ.get("PATH_TRANSLATED")

                # Check if PATH_TRANSLATED is set
                if new_file_path:
                    new_file_path = os.path.join(new_file_path, new_file_name)
                    try:
                        # Create and open the new file for writing
                        with open(new_file_path, "w") as new_file:
                            # Write the content of the original file to the new file
                            new_file.write(file_data)
                        print("Data written to the new file: {}".format(new_file_path))
                    except Exception as e:
                        print("Error creating the new file:", e)
                else:
                    print("PATH_TRANSLATED environment variable is not set.")
            else:
                print("File is empty.")
        else:
            print("File is empty.")
    except Exception as e:
        print("Error:", e)
else:
    print("File not found at:", file_path)
