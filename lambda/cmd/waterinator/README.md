# Deployment

Before deploying the lambda export required environment variables. These are among others credentials you get from the `Security/ Database Access/ MongoDB Users` section in your MongoDB Atlas cockpit:

```bash
export DATABASE_USERNAME={MONGODB_USER_USERNAME}
export DATABASE_PASSWORD={MONGODB_USER_PASSWORD}
export MONGO_URI_FORMAT="mongodb+srv://%s:%s@franpog-cluster-uteiq.mongodb.net/test?retryWrites=true&w=majority"
export DATABASE_NAME="waterinator"
export COLLECTION_NAME="sensordata"
```

Update vendors running:

```bash
export GO111MODULE="on"
go mod init
go mod tidy
go mod vendor
```

To deploy the lambda on GCP run:

```bash
gcloud auth login

export PROJECT_NAME={GCP_PROJECT_NAME}

gcloud functions deploy SaveSensorDataToDatabase --region=us-central1 --runtime go111 --trigger-http --project $PROJECT_NAME --set-env-vars DATABASE_USERNAME=$DATABASE_USERNAME --set-env-vars DATABASE_PASSWORD=$DATABASE_PASSWORD --set-env-vars MONGO_URI_FORMAT=$MONGO_URI_FORMAT --set-env-vars DATABASE_NAME=$DATABASE_NAME --set-env-vars COLLECTION_NAME=$COLLECTION_NAME
```

To make valid test request run:

```bash
curl -X POST {LAMBDA_URL} -d "{\"hardwareID\":123,\"humidityPercent\":42,\"didWater\":1}" -H "Content-type: application/json"
```
