import cgi, cgitb
import sys
from time import sleep

# Create instance of FieldStorage 
form = cgi.FieldStorage() 
# get query string
 values = 