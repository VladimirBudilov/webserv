import os
import sys

# Function to set the HTTP headers for file download
def set_download_headers(file_name):
    print "HTTP/1.1 200 OK\r\nContent-type: image/jpeg\r\nContent-Disposition: attachment; filename=\"{}\"\r\n".format(file_name)

# Get the file path
file_path = os.environ.get("PATH_INFO")  # Use PATH_INFO for the requested file
file_name = os.path.basename(file_path)  # Extract the file name from the path

# Check if the file exists
if os.path.exists(file_path):
    try:
        # Set the appropriate download headers
        set_download_headers(file_name)

        # Open the file for reading in binary mode
        with open(file_path, "r") as file:
            # Read and send the file in smaller chunks
            chunk_size = 4096  # Adjust the chunk size as needed
            while True:
                chunk = file.read(chunk_size)
                if not chunk:
                    break
                sys.stdout.write(chunk)
    except Exception as e:
        print "HTTP/1.1 500 Internal Server Error"
        print "Content-Type: text/html;charset=utf-8"
        print "\r\n"
        print "<H1>Error: {}</H1>".format(e)
else:
    print "HTTP/1.1 404 Not Found"
    print "Content-Type: text/html;charset=utf-8"
    print "\r\n"
    print "<H1>File not found</H1>"
