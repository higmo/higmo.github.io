ZOLA?=zola
BASEDIR=$(CURDIR)
OUTPUTDIR=$(BASEDIR)/public
GITHUB_PAGES_BRANCH=pages

all:
	$(ZOLA) build -o $(OUTPUTDIR)

serve:
	$(ZOLA) serve

github: all
	find . -name '.DS_Store' -delete
	ghp-import -m "Generate site" -b $(GITHUB_PAGES_BRANCH) $(OUTPUTDIR)

publish: github
	git push origin $(GITHUB_PAGES_BRANCH)

.PHONY: all serve