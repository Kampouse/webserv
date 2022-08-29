#!/usr/bin/python

# https://www.geeksforgeeks.org/what-is-cgi-in-python/

# Import CGI module
import cgi

# To create instance of FieldStorage class which we
# can use to work with the submitted form data
form = cgi.FieldStorage()
name = form.getvalue('name')
email = form.getvalue('email')
message = form.getvalue('message')

print ("<html>")
print ("<head>")
print ("<title>CGI Form</title>")
print ("<style>")
print ("body {")
print ("  background-color: #0c2c61;")
print ("}")
print ("h2 {")
print ("  color: #ffffff;")
print ("}")
print ("</style>")
print ("</head>")
print ("<body>")
print ("<h2>Your name : %s</h2>" % (name))
print ("<h2>Your email : %s</h2>" % (email))
print ("<h2>Your message : %s</h2>" % (message))
print ("</body>")
print ("</html>")