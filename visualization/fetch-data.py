# %%
import pymongo
import os
import pandas as pd

def get_mongo_data() -> pd.DataFrame:
    connection_format = os.environ["MONGO_URI_FORMAT"] 
    username = os.environ["DATABASE_USERNAME"] 
    password = os.environ["DATABASE_PASSWORD"] 
    database_name = os.environ["DATABASE_NAME"] 
    collection_name = os.environ["COLLECTION_NAME"] 

    client = pymongo.MongoClient(connection_format.format(username, password))
    db = client[database_name]
    collection = db[collection_name]
    cursor = collection.find({})

    mongo_data = pd.DataFrame(list(cursor))
    return mongo_data


def save_dataframe(data: pd.DataFrame):
    data.to_csv("data.csv", index=False)


def main():
    data = get_mongo_data()
    save_dataframe(data)


if __name__ == "__main__":
    main()


# %%
