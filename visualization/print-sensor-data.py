from pprint import pprint
import pymongo
import os

connection_format = os.environ["MONGO_URI_FORMAT"] 
username = os.environ["DATABASE_USERNAME"] 
password = os.environ["DATABASE_PASSWORD"] 
database_name = os.environ["DATABASE_NAME"] 
collection_name = os.environ["COLLECTION_NAME"] 

client = pymongo.MongoClient(connection_format.format(username, password))
db = client[database_name]
collection = db[collection_name]

cursor = collection.find({})
for document in cursor: 
    pprint(document)
