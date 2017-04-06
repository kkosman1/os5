/*
Main program for the virtual memory project.
Make all of your modifications to this file.
You may add or rearrayange any code or data as you need.
The header files page_table.h and disk.h explain
how to use the page table and disk interfaces.
*/

#include "page_table.h"
#include "disk.h"
#include "program.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

<<<<<<< HEAD
int pageFault=0;
int diskWrite=0;
int diskRead=0;
int counter=0;
int *arrayay;
struct disk *disk;
int check;

void page_fault_handler( struct page_table *pt, int page ){
	int no_pages=page_table_get_npages(pt);
	int no_frames=page_table_get_nframes(pt);
	char *physmen=page_table_get_physmen(pt);
	if (no_frames >= no_pages){
		printf("page fault on page #%d\n",page);
		page_table_set_entry(pt, page, page,PROT_READ|PROT_WRITE);
		pageFault++;
		diskRead=0;
		diskWrite=0;
	}
	else{
		if (check==3){
			pageFault++;
			int temp=page%no_frames;
			if (array[temp]==page){
				page_table_set_entry(pt,page,temp,PROT_READ|PROT_WRITE);
	                        pageFault--;
			}
			else if(array[temp]==-1){
				page_table_set_entry(pt,page,temp,PROT_READ);
				disk_read(disk,page,&physmem[temp*PAGE_SIZE]);
				diskRead++;
			}
			else{
            			disk_write(disk,array[temp],&physmem[temp*PAGE_SIZE]);
                        	disk_read(disk,page,&physmem[temp*PAGE_SIZE]);
                                diskRead++;
                                diskWrite++;
                                page_table_set_entry(pt,page,temp,PROT_READ);
                        }
                        array[temp]=page;
                        page_table_print(pt);
		}	
	else if(check==2){
		pageFault++;
		int k=LinearSearch(0,no_frames-1,page);
 		if(k > -1){
             		page_table_set_entry(pt,page,k,PROT_READ|PROT_WRITE);
                       	counter--;
                        pageFault--;
                }
                else if(array[counter]==-1){
                        page_table_set_entry(pt,page,counter,PROT_READ);
                        disk_read(disk,page,&physmem[counter*PAGE_SIZE]);
                        diskRead++;
                }
		else{
        		disk_write(disk,array[counter],&physmem[counter*PAGE_SIZE]);
        		disk_read(disk,page,&physmem[counter*PAGE_SIZE]);
        		diskRead++;
        		diskWrite++;
                	page_table_set_entry(pt,page,counter,PROT_READ);
        	}
            	array[counter]=page;
            	counter=(counter+1)%no_frames;
            	page_table_print(pt);
        }

        //Implementation of RAND.
        else{
        	pageFault++;
                int k=LinearSearch(0,no_frames-1,page);
        	int temp=lrand48()%no_frames;
        	if(k > -1){
        		page_table_set_entry(pt,page,k,PROT_READ|PROT_WRITE);
            		pageFault--;
            	}
            	else if(counter < no_frames){
            		while(array[temp]!=-1){
            			temp=lrand48()%no_frames;
            			pageFault++;
            		}
            		page_table_set_entry(pt,page,temp,PROT_READ);
            		disk_read(disk,page,&physmem[temp*PAGE_SIZE]);
            		diskRead++;
            		array[temp]=page;
            		counter++;
            	}
            	else{
            		disk_write(disk,array[temp],&physmem[temp*PAGE_SIZE]);
            		disk_read(disk,page,&physmem[temp*PAGE_SIZE]);
            		diskRead++;
            		diskWrite++;
            		page_table_set_entry(pt,page,temp,PROT_READ);
            		array[temp]=page;
            	}
        	page_table_print(pt);
	}
=======
void page_fault_handler( struct page_table *pt, int page )
{
	page_table_set_entry(pt,page,page,PROT_READ|PROT_WRITE);
	printf("page fault on page #%d\n",page);
//	exit(1);
>>>>>>> 1a337f0241d12c8cbdbdf6a30b2b1c408b535011
}

int main( int argc, char *argv[] ){
	if(argc!=5) {
		printf("use: virtmem <npages> <nframes> <rand|fifo|lru|custom> <sort|scan|focus>\n");
		return 1;
	}

	int npages = atoi(argv[1]);
	int nframes = atoi(argv[2]);
	const char *program = argv[4];

	struct disk *disk = disk_open("myvirtualdisk",npages);
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

	char *physmem = page_table_get_physmem(pt);

	if(!strcmp(program,"sort")) {
		sort_program(virtmem,npages*PAGE_SIZE);

	} else if(!strcmp(program,"scan")) {
		scan_program(virtmem,npages*PAGE_SIZE);

	} else if(!strcmp(program,"focus")) {
		focus_program(virtmem,npages*PAGE_SIZE);

	} else {
		fprintf(stderr,"unknown program: %s\n",argv[3]);
		return 1;
	}

	page_table_delete(pt);
	disk_close(disk);

	return 0;
}
