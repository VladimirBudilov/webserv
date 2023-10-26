import os
import sys
from datetime import datetime

def printError(error_str):
    print "HTTP/1.1 500 Internal Server Error\r\nContent-Type: text/html;charset=utf-8\r\n\r\n",
    print "<H1>" + error_str + " doesn't set" + "</H1>"

# Get the file path
file_path = os.environ.get("PATH_INFO")  # Use PATH_INFO for the requested file
file_name = os.path.basename(file_path)  # Extract the file name from the path

# Check if the file exists
if os.path.exists(file_path):
    try:
        # Open the file for reading
        with open(file_path, "r") as file:
            # Read the content of the file
            file_data = file.read()

        if file_data:

            try:
                # Create and open the new file for writing
                with open(file_path, "w") as new_file:
                    # Write response and 200 status in header to the new file
                    print("HTTP/1.1 200 OK\r\n")
                    print("Content-Type: application/octet-stream\r\n")
                    print("Content-Disposition: attachment; filename=" + file_name + "\r\n")
                    print("Content-Length: " + str(len(file_data)) + "\r\n")
                    print("Date: " + str(datetime.now()) + "\r\n")
                    print("\r\n")
                    # Write the file data to the new file
                    new_file.write(file_data)

                print("File downloaded successfully: {}".format(file_name))
                # Finish the response
                print("\r\n\r\n")
            except Exception as e:
                printError("Error creating the new file: {}".format(e))

        else:
            printError("File is empty.")
    except Exception as e:
        printError("Error: {}".format(e))
else:
    printError("File not found at: {}".format(file_path))
