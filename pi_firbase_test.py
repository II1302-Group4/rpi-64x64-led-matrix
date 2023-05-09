from pyrebase import pyrebase
config = {  # define a dictionary named config with several key-value pairs that configure the connection to the database.
    "apiKey": "AIzaSyAhabvmLuMQH8Hw7TKKpBhS6kiWhdps1q0",
    "authDomain": "pixled-17de5.firebaseapp.com",
    "databaseURL": "https://pixled-17de5-default-rtdb.europe-west1.firebasedatabase.app/",
    "storageBucket": "pixled-17de5.appspot.com"
}
firebase = pyrebase.initialize_app(config)
db = firebase.database()

print("Pyrebase database config complete.")

# Function to get data of every LED in grid


def getFullGrid():
    grid = db.child("ledMatrix").get()
    print("Retrieving the full grid...")
    print(grid.val())
    return grid.val()

# Define callback function to handle changes


def convertCharArrayToString(char):
    string = ""
    for x in char:
        string += x
    return string


def handle_change(event):
    pixel_data = event["data"]
    pixel_path = (event["path"])
    event_information = [pixel_path[-1], convertCharArrayToString(pixel_data)]
    if (pixel_path == '/'):
        print("Got first event with only '/' as path")
    else:
        print("[ID, colorValue]")
        print(event_information)


getFullGrid()

# Listen for changes to the path
my_stream = db.child("ledMatrix").stream(handle_change)
print("Stream is ready and listening for changes in database")

# Keep the program running

while True:
    pass
