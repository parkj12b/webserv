import cgi
import os
from urllib.parse import quote, unquote

def escape_html(text):
    return (text
        .replace("&", "&amp;")
        .replace("<", "&lt;")
        .replace(">", "&gt;")
        .replace('"', "&quot;")
        .replace("'", "&#039;"))

def list_directory(directory):

    # Get list of files and directories
    items = os.listdir(directory)
    items.sort()
    
    # Create HTML content
    html = ''
    html += '<html><head><title>Directory Listing</title><meta charset="utf-8"></head>'
    html += '<body><h1>Directory Listing</h1><ul>'
    
    # Parent directory link
    parent_dir = os.path.dirname(directory)
    if parent_dir:
        parent_dir = quote(parent_dir)
        html += f'<li><a href="../">../</a></li>'
    
    # List files and directories
    for item in items:
        item_path = os.path.join(directory, item)
        item_url = quote(item)
        if os.path.isdir(item_path):
            item_url += '/'
        html += f'<li><a href="{item_url}">{escape_html(item_url)}</a></li>'
    
    html += '</ul></body></html>'
    return html

def main():
    # Handle the CGI request
    directory = os.environ.get('PATH_INFO')
    
    if directory == None:
        print("Status: 500\r")
        exit(0)

    print("Content-Type: text/html\r")
    print("Status: 200\r")
    # Generate the directory listing HTML
    html = list_directory(directory)
    print(html)

if __name__ == '__main__':
    main()