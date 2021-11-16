ZOLA ?= zola
GITHUB_PAGES_BRANCH ?= pages
OUTPUT_DIR ?= $(CURDIR)/public

all:
	$(ZOLA) build -o $(OUTPUT_DIR)
	$(MAKE) -C thoughtwriter OUTPUT_DIR=$(OUTPUT_DIR)/thoughtwriter

serve:
	$(ZOLA) serve

github: all thoughtwriter
	find . -name '.DS_Store' -delete
	ghp-import -m "Generate site" -b $(GITHUB_PAGES_BRANCH) $(OUTPUT_DIR)

publish: github
	git push origin $(GITHUB_PAGES_BRANCH)

.PHONY: all serve