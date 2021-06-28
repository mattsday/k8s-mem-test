FROM debian:buster AS build
RUN apt-get update && apt-get -y install gcc
COPY memtest.c /memtest.c
RUN gcc /memtest.c -o /memtest
RUN chmod +x /memtest

FROM debian:buster
COPY --from=build /memtest /memtest
ENTRYPOINT ["/memtest"]
