include config.mk

APP_URL ?= https://ekzeget.ru/bible/
APP_NAME ?= my-app
VERSION ?= 1.0
CATEGORIES ?= Utility
ICON_SRC ?= icon.png

APP_ID := $(shell echo "$(APP_NAME)" | sed -e 's/^[[:space:]-]*//' -e 's/[[:space:]-]*$$//' | tr '[:upper:]' '[:lower:]' | tr ' ' '_')
APP_URL := $(shell echo "$(APP_URL)" | sed -e 's/^[[:space:]-]*//' -e 's/[[:space:]-]*$$//')
APP_NAME := $(shell echo "$(APP_NAME)" | sed -e 's/^[[:space:]-]*//' -e 's/[[:space:]-]*$$//')
BUILD_DIR = build
PREFIX ?= $(HOME)/.local
DATADIR ?= $(PREFIX)/share/$(APP_ID)
DESKTOPDIR ?= $(PREFIX)/share/applications

WYEBADBLOCK_DIR := $(HOME)/.config/wyebadblock
WYEBADBLOCK_SO := $(WYEBADBLOCK_DIR)/adblock.so
EASYLIST := $(WYEBADBLOCK_DIR)/easylist.txt
WYEBADBLOCK_SRC := src/wyebadblock

initial_clean := $(shell rm -rf "$(BUILD_DIR)")

check-wyebadblock: $(WYEBADBLOCK_SO)

$(WYEBADBLOCK_SO): $(WYEBADBLOCK_SRC)/ab.c
	@echo "сборка wyebadblock..."
	cd $(WYEBADBLOCK_SRC) && WEBKITVER=4.1 make
	mkdir -p $(WYEBADBLOCK_DIR) $(HOME)/.local/bin
	cp $(WYEBADBLOCK_SRC)/wyebab $(HOME)/.local/bin/
	cp $(WYEBADBLOCK_SRC)/adblock.so $(WYEBADBLOCK_DIR)/
	test -f $(EASYLIST) || wget -nv --show-progress -qO $(EASYLIST) \
		https://easylist.to/easylist/easylist.txt


all: check-wyebadblock $(BUILD_DIR)/$(APP_ID) $(BUILD_DIR)/$(APP_ID).desktop

$(BUILD_DIR)/icon.png:
	mkdir -p $(BUILD_DIR)
ifneq (,$(findstring http://,$(ICON_SRC))$(findstring https://,$(ICON_SRC)))
	wget -q -O $@ $(ICON_SRC)
else
	cp $(ICON_SRC) $@
endif


$(BUILD_DIR)/$(APP_ID): src/app.cpp src/webview.h $(BUILD_DIR)/icon.png
	mkdir -p $(BUILD_DIR)
	sed -i -e 's#^\(constexpr const char\* APP_URL = \).*#\1"$(APP_URL)";#' \
           -e 's#^\(constexpr const char\* APP_NAME = \).*#\1"$(APP_NAME)";#' $<
	c++ $< `pkg-config --cflags --libs gtk+-3.0 webkit2gtk-4.1 gio-2.0` -std=c++17 -o $@

$(BUILD_DIR)/$(APP_ID).desktop:
	mkdir -p $(BUILD_DIR)
	@echo "[Desktop Entry]" > $@
	@echo "Type=Application" >> $@
	@echo "Version=$(VERSION)" >> $@
	@echo "Name=$(APP_NAME)" >> $@
	@echo "Exec=$(DATADIR)/$(APP_ID)" >> $@
	@echo "Icon=$(DATADIR)/icon.png" >> $@
	@echo "Categories=$(CATEGORIES);" >> $@
	@echo "Comment=$(APP_NAME) WebView Application" >> $@
	@echo "Terminal=false" >> $@

install: all
	install -Dm755 $(BUILD_DIR)/$(APP_ID) $(DESTDIR)$(DATADIR)/$(APP_ID)
	install -Dm644 $(BUILD_DIR)/icon.png $(DESTDIR)$(DATADIR)/icon.png

	install -Dm644 $(BUILD_DIR)/$(APP_ID).desktop $(DESTDIR)$(DESKTOPDIR)/$(APP_ID).desktop


uninstall:
	rm -rf $(DESTDIR)$(DATADIR)
	rm -f $(DESTDIR)$(DESKTOPDIR)/$(APP_ID).desktop

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all install uninstall clean
