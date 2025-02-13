FROM --platform=linux/amd64 aflplusplus/aflplusplus:latest

RUN apk add --no-cache gdb git python3 py3-pip && \
    python3 -m pip install --upgrade pip && \
    git clone https://github.com/hugsy/gef.git /opt/gef && \
    echo "source /opt/gef/gef.py" > ~/.gdbinit

RUN apk add --no-cache \
    binutils \
    file \
    grep \
    less \
    ltrace \
    strace \
    tmux
