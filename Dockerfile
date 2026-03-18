FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    build-essential \
    qtbase5-dev \
    qtchooser \
    qt5-qmake \
    qtbase5-dev-tools \
    sed \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY . .

# Исправляем ошибку компиляции
RUN sed -i 's/return users.keys();/return users.keys().toVector();/' database.cpp

# Проверяем, что исправили
RUN grep -n "toVector" database.cpp || echo "Не удалось исправить!"

# Собираем проект
RUN qmake echoServer.pro && make clean && make

EXPOSE 33333

CMD ["./echoServer"]