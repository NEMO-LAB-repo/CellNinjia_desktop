docker container prune --force
docker image prune --force
docker build -t cellninja:1.0 .
docker run -it --name cellninja --privileged -v /dev/bus/usb:/dev/bus/usb cellninja:1.0 bash
