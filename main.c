#include "page_table.h"
#include "disk.h"
#include "program.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int *arrayPages;
struct disk *disk;
int checker;
int pageFault=0;
int diskRead=0;
int diskWrite=0;
int counter=0;
int evict=-1;
int temp=-1;

void printResults(){
	printf("SUMMARY:\n");
	printf("No. of page faults:%d\n",pageFault);
	printf("No. of disk reads:%d\n",diskRead);
	printf("No. of disk writes:%d\n",diskWrite);
}

void page_fault_handler( struct page_table *pt, int page ){
	int nframes=page_table_get_nframes(pt);
	char *physmem = page_table_get_physmem(pt);

	int frame, bits;
	page_table_get_entry(pt, page, &frame, &bits);
	if(bits>=3){
		page_table_set_entry(pt,page,frame,PROT_READ|PROT_WRITE|PROT_EXEC);
	}
	else if (bits>=1){
		page_table_set_entry(pt,page,frame,PROT_READ|PROT_WRITE);
	}
	else {
		pageFault++;
		if(counter < nframes){
			page_table_set_entry(pt,page,counter,PROT_READ);
			disk_read(disk,page,&physmem[counter*PAGE_SIZE]);
		        arrayPages[counter]=page;
			counter++;
			diskRead++;
		}
		else{
			//custom implementation
			if(checker==3){
				if (temp==-1){
					evict=(evict+1)%nframes;
				}
				else{
					evict=temp;
					temp=-1;
				}
			}
			//FIFO implementation
			else if(checker==2){
				evict=(evict+1)%nframes;
			}
			//RAND implementation
			else{
				evict=rand()%nframes;
			}

			page_table_get_entry(pt, arrayPages[evict], &frame, &bits);
			if(bits>=3){
				disk_write(disk,arrayPages[evict],&physmem[evict*PAGE_SIZE]);
				page_table_set_entry(pt,arrayPages[evict],0,0);
				diskRead++;
				diskWrite++;
				page_table_set_entry(pt,page,evict,PROT_READ);
				disk_read(disk,page,&physmem[evict*PAGE_SIZE]);
				arrayPages[evict]=page;
				temp=evict;
			}
			else {
				page_table_set_entry(pt,page,evict,PROT_READ);
				disk_read(disk,page,&physmem[evict*PAGE_SIZE]);
				diskRead++;
				arrayPages[evict]=page;
				temp=evict;
			}
			//page_table_print(pt);
		}
	}
}

int main( int argc, char *argv[] ){
	if(argc!=5) {
		printf("use: virtmem <npages> <nframes> <rand|fifo|custom> <sort|scan|focus>\n");
		return 1;
	}

	int npages = atoi(argv[1]);
	int nframes = atoi(argv[2]);

	if(!strcmp(argv[3],"rand")){
		checker=1;
	}
	else if(!strcmp(argv[3],"fifo")){
		checker=2;
	}
	else{
		checker=3;
	}
	const char *program = argv[4];

	arrayPages=(int *)malloc(nframes*sizeof(int));
	int i;
	for(i=0;i<nframes;i++){
		arrayPages[i]=-1;
	}

	disk = disk_open("myvirtualdisk",npages);

	if(!disk) {
		fprintf(stderr,"couldn't create virtual disk: %s\n",strerror(errno));
		return 1;
	}

	struct page_table *pt = page_table_create( npages, nframes, page_fault_handler );
	if(!pt) {
		fprintf(stderr,"couldn't create page table: %s\n",strerror(errno));
		return 1;
	}

	char *virtmem = page_table_get_virtmem(pt);

	if(!strcmp(program,"sort")) {
		sort_program(virtmem,npages*PAGE_SIZE);
	} else if(!strcmp(program,"scan")) {
		scan_program(virtmem,npages*PAGE_SIZE);

	} else if(!strcmp(program,"focus")) {
		focus_program(virtmem,npages*PAGE_SIZE);

	} else {
		fprintf(stderr,"unknown program: %s\n",argv[3]);
	}

	page_table_delete(pt);
	disk_close(disk);
	printResults();
	return 0;
}
