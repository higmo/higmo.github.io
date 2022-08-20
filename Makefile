ZOLA ?= zola
GITHUB_PAGES_BRANCH ?= pages
OUTPUT_DIR ?= $(CURDIR)/public

all:
	$(ZOLA) build -o $(OUTPUT_DIR)

serve:
	$(ZOLA) serve

github: all
	find . -name '.DS_Store' -delete
	ghp-import -m "Generate site" -b $(GITHUB_PAGES_BRANCH) $(OUTPUT_DIR)

publish: github
	git push origin $(GITHUB_PAGES_BRANCH)

.PHONY: all serve
