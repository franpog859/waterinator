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

gcloud functions deploy SaveSensorDataToDatabase --runtime go111 --trigger-http --project $PROJECT_NAME --set-env-vars DATABASE_USERNAME=$DATABASE_USERNAME DATABASE_PASSWORD=$DATABASE_PASSWORD MONGO_URI_FORMAT=$MONGO_URI_FORMAT DATABASE_NAME=$DATABASE_NAME COLLECTION_NAME=$COLLECTION_NAME
```
