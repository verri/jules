all:
	standardese --output.advanced_code_block=0 -I ../jules ../jules/jules
	sh check_files.sh
