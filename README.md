# Waterinator

> TODO: add here some logo with a leaf and a drop on it

This summer I started to grow cucumbers. It was fun but after a week it started to be monotonous and not as fun as at the beginning. I discovered that remembering about watering the plants is sooo hard. And that's how Waterinator was born.

Waterinator is an IoT project which combines low level C code for Espressif chip, Go code running in the cloud and Python scripts used for playing around with the generated data. 

## EMBEDDED

Processor checks the humidity of the plant's ground. If it is too dry, chip waters it using a water pump. After that it makes a request to the server with all gathered data and sleeps for a while. It's important to tune the sensor and humidity threshold to make it work correctly. Second even more important thing is to be really sure that the pump is off after the watering.

## LAMBDA 

Go code deployed in Google Cloud Platform as a Function runs only when it is triggered. This architecture is known as FAAS. It just generates lower cost. The function connects to the MongoDB database and pass the enriched data there.

## VISUALIZATION

Python scripts which fetch the data from the database and generate pleasant plots. Diagrams could be used to analyse the data and to understand how the whole project works in real time.

> TODO: add here some good looking plot

### Development

If you want to develop the project or build one for yourself go to the `embedded/`, `lambda/` and `visualization/` directories. There are some development hints as well as requirements to run the project. You will find there notes how to deploy the function on GCP, compile embedded code or plot some diagrams from the database.

Feel free to open an issue or fork and create pull request! I'm sure there are plenty of things that could be done better. Just let me notice them.

