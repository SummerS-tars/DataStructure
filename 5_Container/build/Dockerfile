# 步骤 1: 选择基础镜像
FROM gcc:latest

# 步骤 2: 将 APT 源更换为国内镜像源 (清华大学 TUNA)
RUN cat <<EOF > /etc/apt/sources.list.d/debian.sources
Types: deb
URIs: https://mirrors.tuna.tsinghua.edu.cn/debian
Suites: bookworm bookworm-updates bookworm-backports
Components: main contrib non-free non-free-firmware
Signed-By: /usr/share/keyrings/debian-archive-keyring.gpg

Types: deb
URIs: https://mirrors.tuna.tsinghua.edu.cn/debian-security
Suites: bookworm-security
Components: main contrib non-free non-free-firmware
Signed-By: /usr/share/keyrings/debian-archive-keyring.gpg
EOF

# 步骤 3: 更新包管理器并安装 zsh 及其他工具
RUN apt-get update && apt-get install -y \
    zsh \
    git \
    cmake \
    gdb \
    vim \
    curl \
    && rm -rf /var/lib/apt/lists/*

# 步骤 4 (修正): 重新加入 oh-my-zsh 的安装脚本
# 这是创建 .zshrc 文件的关键步骤，必须在修改 .zshrc 之前执行
RUN sh -c "$(curl -fsSL https://raw.githubusercontent.com/ohmyzsh/ohmyzsh/master/tools/install.sh)" "" --unattended

# 步骤 5: 安装 powerlevel10k 主题
RUN git clone --depth=1 https://github.com/romkatv/powerlevel10k.git /root/.oh-my-zsh/custom/themes/powerlevel10k

# 步骤 6: 设置 zsh 主题为 powerlevel10k
# 此时 .zshrc 文件已经由 oh-my-zsh 创建，所以 sed 命令可以成功执行
RUN sed -i 's|ZSH_THEME=".*"|ZSH_THEME="powerlevel10k/powerlevel10k"|' /root/.zshrc

# 步骤 7: 复制预设的 p10k 配置文件，跳过首次启动的交互式向导
COPY .p10k.zsh /root/.p10k.zsh

# 步骤 8: 将 zsh 设置为 root 用户的默认登录 Shell
RUN chsh -s /bin/zsh

# 步骤 9: 设置容器的默认启动命令
CMD ["/bin/zsh"]