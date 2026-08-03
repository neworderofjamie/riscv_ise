

.PHONY: all common compiler_frontend frontend fenn_backend

all: common compiler_frontend frontend

common:
	$(MAKE) -C src/common

compiler_frontend:
	$(MAKE) -C src/compiler_frontend

frontend:
	$(MAKE) -C src/frontend

fenn_backend:
	$(MAKE) -C src/fenn all
