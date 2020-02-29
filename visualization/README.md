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

## Figures

Here is what you can read from the figures and what are they used for:

- `humidity_time.png` - shows how the humidity changes in time and marks points when the chip watered the ground. It's used to control overall work of the project and to tune initial threshold for humidity
- `did_water.png` - shows how many times the chip took an action and watered the plant and how many it just mesured the humidity and just waited. It's used to tune a wait time of the chip
- `when_water.png` - shows at which percent of the humidity the chip waters the plant. It's used to tume humidity threshold
