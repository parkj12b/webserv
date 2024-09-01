#!/usr/bin/env python3

import cgi
import cgitb
import os
from datetime import datetime
import sys

# Enable CGI error reporting
cgitb.enable()

http_status_codes = {
    # 4xx Client Error
    400: "Bad Request",
    401: "Unauthorized",
    402: "Payment Required",
    403: "Forbidden",
    404: "Not Found",
    405: "Method Not Allowed",
    406: "Not Acceptable",
    407: "Proxy Authentication Required",
    408: "Request Timeout",
    409: "Conflict",
    410: "Gone",
    411: "Length Required",
    412: "Precondition Failed",
    413: "Payload Too Large",
    414: "URI Too Long",
    415: "Unsupported Media Type",
    416: "Range Not Satisfiable",
    417: "Expectation Failed",
    418: "I'm a teapot",  # Note: This is an April Fools' joke from RFC 2324
    421: "Misdirected Request",
    422: "Unprocessable Entity",
    423: "Locked",
    424: "Failed Dependency",
    426: "Upgrade Required",
    428: "Precondition Required",
    429: "Too Many Requests",
    431: "Request Header Fields Too Large",
    451: "Unavailable For Legal Reasons",

    # 5xx Server Error
    500: "Internal Server Error",
    501: "Not Implemented",
    502: "Bad Gateway",
    503: "Service Unavailable",
    504: "Gateway Timeout",
    505: "HTTP Version Not Supported",
    506: "Variant Also Negotiates",
    507: "Insufficient Storage",
    508: "Loop Detected",
    510: "Not Extended"
}

try:

    if 'ERROR_CODE' in os.environ:
        error_code = os.environ.get("ERROR_CODE")
    else:
        error_code = 500
    # Header
    if error_code == None:
        print(f"Status: 500\r")
        exit(0)
    else:
        print(f"Status: 200\r")
    print("content-type: text/html\r")

    # Start HTML output
    print("<!DOCTYPE html>")
    print("<html lang='en'>")
    print("<head>")
    print("<meta charset='UTF-8'>")
    print("<title>Error Page</title>")
    print("<style>")
    print("body { font-family: Arial, sans-serif; margin: 20px; }")
    print(".container { max-width: 800px; margin: auto; }")
    print("h1 { color: #d9534f; }")
    print("p { font-size: 18px; }")
    print(".error-code { font-size: 24px; font-weight: bold; }")
    print("</style>")
    print("</head>")
    print("<body>")

    # Generate the error message
    print("<div class='container'>")
    print("<h1>Error Page</h1>")
    print("<p>We're sorry, but something went wrong.</p>")

    # Capture the error details from the environment
    print(f"<p>Error code: {error_code}</p>")
    error_msg = f"{error_code} {http_status_codes[int(error_code)]}"  # Default error code

    print(f"<p class='error-code'>{error_msg}</p>")
    print(f"<p>Time of error: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}</p>")
    print("<div>")
    print(f"<img src='https://http.cat/{error_code}' alt='HTTPCats error img' width='100%'>")
    print("</div>")
    print("<a href='/'>&larr; Back to Home</a>")

    # End HTML output
    print("</div>")
    print("</body>")
    print("</html>")
except Exception as e:
    print("errorPage exception", file=sys.stderr)
    sys.exit(255)