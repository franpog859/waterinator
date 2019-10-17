# Development

```bash
export GO111MODULE="on"
go mod init
go mod tidy
go mod vendor
gcloud functions deploy SaveSensorDataToDatabase --runtime go111 --trigger-http --project {PROJECT_NAME} --set-env-vars DATABASE_PASSWORD={MONGODB_USER_PASSWORD}
```
