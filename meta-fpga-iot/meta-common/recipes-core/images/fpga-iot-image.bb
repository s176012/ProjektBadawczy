DESCRIPTION = "Base FPGA IoT image, based on core-image."

IMAGE_FEATURES += " splash ssh-server-openssh "

IMAGE_INSTALL = " \
	packagegroup-core-boot \
	packagegroup-core-full-cmdline \
	${CORE_IMAGE_EXTRA_INSTALL} \
"

inherit core-image

IMAGE_INSTALL:append = " \
	socat i2c-tools mtd-utils \
	bash coreutils util-linux devmem2 systemd dhcpcd \
	libgpiod libgpiod-tools test-app \
"

# For MQTT
IMAGE_INSTALL:append = " \
	mosquitto \
"

# For gcc, g++, make, stdlib, etc.
IMAGE_INSTALL:append = " \
	packagegroup-core-buildessential \
	cmake \
"

# For Python3 and packages
IMAGE_INSTALL:append = " \
	python3 python3-pip python3-cryptography  python3-numpy python3-paho-mqtt \
"
# For Python3 and packages
IMAGE_INSTALL:append = " \
	vim wireshark curl git libsqlite3-dev python3-dev libcereal-dev uthash-dev mosquitto-clients openssh-ssh dtc tcpdump apache2\
"

IMAGE_FEATURES:append = " "

IMAGE_FSTYPES += " jffs2 squashfs-xz"
