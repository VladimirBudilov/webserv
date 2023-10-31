function sendPostRequest() {
    var button = document.getElementById('button');
    var result = document.getElementById('result');
    var fileInput = document.getElementById('fileInput');

    button.addEventListener('click', async () => {
        // Get the selected file from the file input
        var file = fileInput.files[0];

        // Check if a file was selected
        if (!file) {
            result.innerText = 'Please select a file to upload.';
            return;
        }
        // Create a FileReader to read the file contents
        var reader = new FileReader();

        reader.onload = async function (event) {
            var fileContent = event.target.result;

            // Add the file name with extension and a space to the beginning of the content
            fileContent = file.name + ' ' + fileContent;

            var options = {
                method: 'POST',
                headers: {
                    'Content-Type': 'text/plain',
                },
                body: fileContent, // Use the modified file content as the body
            };

            try {
                const resp = await fetch('/bin-cgi/uploader.py/data', options);
                if (resp.ok) {
                    result.innerText = await resp.text();
                } else {
                    console.error('Request failed with status:', resp.status);
                }
            } catch (error) {
                console.error('An error occurred:', error);
            }
        };
        // Read the file as text (you can choose a different type based on your file content)
        reader.readAsText(file);
    });
}

// Call the function to set up the event listener
sendPostRequest();