# Demand-Paging-Simulator

Note: In the dockerfile, last 3 lines have "changeme" and I'd advise you do. As well, this is in the echo command in the shell script too. It doesn't break anything if you don't but just looks strange.

## Run Commands:
NOTE: If any folder in your pathing has spaces, ensure you place quotes around the -v command. This goes for all OS's. For example:
```
docker run --name changeme -v "C:\My Drive\<rest>:/home/assignments/changeme" -t changeme
```
# --- Docker ---
## --- Linux ---
```
docker build -t changeme:latest -f docker/Dockerfile .
docker run --name changeme -v <location>:/home/assignments/changeme -t changeme
```
When coming back (the --name assigns a name to a container, so no need to do it again since you've already made the container)
```
docker run -v <location>:/home/assignments/changeme -t changeme
```
To clean up your system after you're done:
```
docker system prune --all
```
## --- Windows ---
```
docker build -t changeme:latest -f Docker/Dockerfile .
docker run --name changeme -v <drive>:<location>:/home/assignments/changeme -t changeme
```
When coming back (the --name assigns a name to a container, so no need to do it again since you've already made the container)
```
docker run -v <drive>:<location>:/home/assignments/changeme -t changeme
```
To clean up your system after you're done:
```
docker system prune --all
```
# --- Podman ---
## --- Linux ---
```
podman build -t changeme:latest -f docker/Dockerfile .
podman run --name changeme -v <location>:/home/assignments/changeme:z -t changeme
```
When coming back (the --name assigns a name to a container, so no need to do it again since you've already made the container)
```
podman run -v <location>:/home/assignments/changeme:z -t changeme
```
To clean up your system after you're done:
```
podman system prune --all
```
## --- Windows ---
```
podman build -t changeme:latest -f Docker/Dockerfile .
podman run --name changeme -v <drive>:<location>:/home/assignments/changeme -t changeme
```
When coming back (the --name assigns a name to a container, so no need to do it again since you've already made the container)
```
podman run -v <drive>:<location>:/home/assignments/changeme -t changeme
```
To clean up your system after you're done:
```
podman system prune --all
```
