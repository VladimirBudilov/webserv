def generate_html_page():
    html = """
    <!DOCTYPE html>
    <html>
    <head>
        <title>example HTML</title>
    </head>
    <body>
        <h1>yo</h1>
        <p>Python.</p>
    </body>
    </html>
    """
    return html

if __name__ == "__main__":
    html_response = generate_html_page()
    print(html_response)
