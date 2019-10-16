package main

import (
	"net/http"

	"github.com/gin-gonic/gin"
)

const (
	externalPort = ":8000"
)

func main() {
	gin.SetMode(gin.ReleaseMode)

	externalRouter := gin.Default()
	externalRouter.GET("/", func(context *gin.Context) {
		context.JSON(http.StatusOK, "Hello!")
	})

	externalRouter.Run(externalPort)
}
