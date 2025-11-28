.PHONY: all synth report clean

OUTDIR := outputs

all: synth report

synth:
	./synth.sh

report:
	python3 tools/parse_yosys_stats.py $(OUTDIR)/flexpipe.json > $(OUTDIR)/yosys_report.json

clean:
	rm -rf $(OUTDIR)/*.json $(OUTDIR)/*.v $(OUTDIR)/*.log
