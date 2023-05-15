from pyrebase import pyrebase
from collections.abc import Mapping
import time

print("inside py")
config = {  # define a dictionary named config with several key-value pairs that configure the connection to the database.
    "apiKey": "AIzaSyAhabvmLuMQH8Hw7TKKpBhS6kiWhdps1q0",
    "authDomain": "pixled-17de5.firebaseapp.com",
    "databaseURL": "https://pixled-17de5-default-rtdb.europe-west1.firebasedatabase.app/",
    "storageBucket": "pixled-17de5.appspot.com"
}
firebase = pyrebase.initialize_app(config)
db = firebase.database()

grid = db.child("ledMatrix").get()
print("Retrieve the full grid")
print(grid.val())