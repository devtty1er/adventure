FROM --platform=linux/amd64 aflplusplus/aflplusplus:latest

RUN apt-get update && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y \
    gdb \
    git \
    python3 \
    python3-pip \
    binutils \
    file \
    grep \
    less \
    ltrace \
    strace \
    tmux && \
    rm -rf /var/lib/apt/lists/*

ENV LC_CTYPE=C.UTF-8

RUN python3 -m pip install --upgrade pip && \
    git clone https://github.com/hugsy/gef.git /opt/gef && \
    echo "source /opt/gef/gef.py" > /home/vscode/.gdbinit && \
    chown -R vscode:vscode /home/vscode/.gdbinit
