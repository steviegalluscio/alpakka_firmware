# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2022, Input Labs Oy.

CMAKE_ARGS = -DDEVICE=$(DEVICE) -UCFG_TICK_FREQUENCY -UCFG_IMU_TICK_SAMPLES
ifdef CFG_TICK_FREQUENCY
	CMAKE_ARGS += -DCFG_TICK_FREQUENCY=$(CFG_TICK_FREQUENCY)
endif
ifdef CFG_IMU_TICK_SAMPLES
	CMAKE_ARGS += -DCFG_IMU_TICK_SAMPLES=$(CFG_IMU_TICK_SAMPLES)
endif

default: version
	mkdir -p build
	cmake . -B build $(CMAKE_ARGS) && cd build && make -j16

rebuild: version
	cd build && make -j16

version:
	#sh -e scripts/version.sh
	printf "/* Version pinned for CTRL app compatibility */\n#define VERSION \"1.2.0\"\n" > src/headers/version.h

install:
	sh -e scripts/install.sh $(DEVICE)

clean:
	rm -rf build
	rm -f src/headers/version.h

load:
	sh -e scripts/load.sh

reload: rebuild load

session:
	sh -e scripts/session.sh

session_quit:
	screen -S alpakka -X quit

restart:
	screen -S alpakka -X stuff R

bootsel:
	screen -S alpakka -X stuff B

calibrate:
	screen -S alpakka -X stuff C

factory:
	screen -S alpakka -X stuff F

reset_config:
	screen -S alpakka -X stuff N

reset_profiles:
	screen -S alpakka -X stuff P

test:
	screen -S alpakka -X stuff T
