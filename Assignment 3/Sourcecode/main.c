/*
Main program for the virtual memory project.
Make all of your modifications to this file.
You may add or rearrange any code or data as you need.
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

char *physmem;

struct disk *disk;
int npages, nframes;
int *loaded_pages, *clock;
int pageswap, fifo_counter, fault_counter = 0;

void print_second_chance()
{
	int i;
	printf("+---+---+\n");
	for(i = 0; i < nframes; i++)
	{
		if(fifo_counter == i)
		{
			printf("| %d | %d | <-\n", loaded_pages[i], clock[i]);
		}
		else
		{
			printf("| %d | %d |\n", loaded_pages[i], clock[i]);
		}
		printf("+---+---+\n");	
	}
}

void get_swap_frame(int *vFrame)
{
	int i;
	switch(pageswap)
	{
		case 0:
			*vFrame = lrand48() % nframes;
			return;
		case 1:
			*vFrame = fifo_counter;
			fifo_counter++;
			fifo_counter = fifo_counter % nframes;
			return;
		case 2:
			print_second_chance();
			i = fifo_counter;
			int do_repeat = 1;
			while(do_repeat == 1)
			{
				//check if it's reference bit is 0
				if(clock[i] == 0)
				{
					do_repeat = 0;
					*vFrame = i;
					fifo_counter++;
					fifo_counter = fifo_counter % nframes;
					clock[i] = 1;
				}
				else
				{
					clock[i] = 0;
					i++;
					i = i % nframes;
					//second chance used
					if(i == fifo_counter)
					{
						do_repeat = 0;
						*vFrame = fifo_counter;
						fifo_counter++;
						fifo_counter = fifo_counter % nframes;
						clock[i] = 1;
					}
				}
			}
			return;
	}
}

void page_fault_handler( struct page_table *pt, int page )
{
	fault_counter++;
	int flag;
	int frame;

	//get frame and flag for the page
	page_table_get_entry(pt, page, &frame, &flag);
	page_table_print_entry(pt,page);
	int i;
	switch(flag)
	{
		case 0:
			//check for free frame
			for(i = 0; i < nframes; i++)
			{
				if(loaded_pages[i] == -1)
				{
					//read from disk to physmem
					page_table_set_entry(pt, page, i, PROT_READ);
					disk_read(disk, page, &physmem[i*PAGE_SIZE]);
					loaded_pages[i] = page;

					page_table_print_entry(pt,page);
					printf("\n");
					if(pageswap == 2)
					{			
						clock[i] = 1;
					}
					return;
				}
			}
			printf("SIDESWAPPING\n");
			//variables for victim
			int vFrame, vPage, vFlag;

			//get the victim frame
			get_swap_frame(&vFrame);

			//set the victim page
			vPage = loaded_pages[vFrame];

			//get the victim flag
			page_table_get_entry(pt, vPage, &vFrame, &vFlag);

			//check for RW flag			
			int rw = (PROT_READ|PROT_WRITE);
			if(vFlag == rw)
			{
				//write victim from physmem to disk
				disk_write(disk, vPage, &physmem[vFrame*PAGE_SIZE]);
			}

			//read from disk to victim frame
			disk_read(disk, page, &physmem[vFrame*PAGE_SIZE]);

			//update page table entries
			page_table_set_entry(pt, page, vFrame, PROT_READ);
			page_table_set_entry(pt, vPage, 0, 0);
			page_table_print_entry(pt,page);
			printf("\n");
			//update loaded_pages
			loaded_pages[vFrame] = page;

			if(pageswap == 2)
			{			
				clock[vFrame] = 1;
			}
			
			return;
		case PROT_READ:
			page_table_set_entry(pt, page, frame, PROT_READ|PROT_WRITE);
			page_table_print_entry(pt,page);
			printf("\n");
			if(pageswap == 2)
			{			
				clock[frame] = 1;
			}
			return;
	}
	printf("page fault on page #%d\n",page);
	exit(1);
}

int main( int argc, char *argv[] )
{
	if(argc!=5) 
	{
		printf("use: virtmem <npages> <nframes> <rand|fifo|custom> <sort|scan|focus>\n");
		return 1;
	}

	npages = atoi(argv[1]);
	nframes = atoi(argv[2]);
	const char *algorithm = argv[3];
	const char *program = argv[4];

	loaded_pages = malloc(sizeof(int) * nframes);
	int i;
	for(i = 0; i < nframes; i++)
	{
		//indicate that there is no pages loaded yet
		loaded_pages[i] = -1;
	}

	disk = disk_open("myvirtualdisk",npages);
	if(!disk) 
	{
		fprintf(stderr,"couldn't create virtual disk: %s\n",strerror(errno));
		return 1;
	}

	struct page_table *pt = page_table_create( npages, nframes, page_fault_handler );
	if(!pt) 
	{
		fprintf(stderr,"couldn't create page table: %s\n",strerror(errno));
		return 1;
	}
	
	char *virtmem = page_table_get_virtmem(pt);

	physmem = page_table_get_physmem(pt);

	if(!strcmp(algorithm, "rand")) 
	{
		pageswap = 0;
	}
	else if(!strcmp(algorithm, "fifo"))
	{
		pageswap = 1;
		fifo_counter = 0;
	}
	else if(!strcmp(algorithm, "custom"))
	{
		pageswap = 2;
		fifo_counter = 0;
		clock = malloc(sizeof(int) * nframes);
		for(i = 0; i < nframes; i++)
		{
			clock[i] = 0;
		}
	}
	else
	{
		fprintf(stderr,"unknown algorithm: %s\n",argv[2]);
	}

	if(!strcmp(program,"sort")) 
	{
		sort_program(virtmem,npages*PAGE_SIZE);

	} 
	else if(!strcmp(program,"scan")) 
	{
		scan_program(virtmem,npages*PAGE_SIZE);

	} 
	else if(!strcmp(program,"focus")) 
	{
		focus_program(virtmem,npages*PAGE_SIZE);

	} 
	else 
	{
		fprintf(stderr,"unknown program: %s\n",argv[3]);
	}
	printf("Faults: %d\n", fault_counter);
	page_table_delete(pt);
	disk_close(disk);

	return 0;
}
