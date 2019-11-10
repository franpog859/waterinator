# Development and usage

To run virtual environment and install dependencies run:

```bash
virtualenv .env && source .env/bin/activate && pip install -r requirements.txt
```

Before running the script export required environment variables. These are among others credentials you get from the `Security/ Database Access/ MongoDB Users` section in your MongoDB Atlas cockpit:

```bash
export DATABASE_USERNAME={DATABASE_USERNAME}
export DATABASE_PASSWORD={DATABASE_PASSWORD}
export MONGO_URI_FORMAT="mongodb+srv://{}:{}@franpog-cluster-uteiq.mongodb.net/test?retryWrites=true&w=majority"
export DATABASE_NAME="waterinator"
export COLLECTION_NAME="sensordata"
```

To fetch the data, print and save figures run:

```bash
python fetch-data.py
python print-figures.py
```

To save the requirements after development run:

```bash
pip freeze > requirements.txt
```

To exit the virtual environment run:

```bash
deactivate
```
