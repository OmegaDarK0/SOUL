NAME    := SOUL
VERSION := 0.1.0-alpha

include common.mk

PUBLIC_HEADERS := $(INC_DIR)/void.h \
                  $(INC_DIR)/soul.hpp \
                  $(INC_DIR)/ecs.hpp \
                  $(INC_DIR)/components.hpp \
                  $(INC_DIR)/math.hpp \
                  $(INC_DIR)/asset.hpp

LDLIBS += -lVOID
