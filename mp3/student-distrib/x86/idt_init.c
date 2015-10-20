#include <stddef.h>
#include <inc/x86/idt_init.h>
#include <inc/x86/idt_table.h>
#include <inc/x86/err_handler.h>

void interrupt_handler_with_number (size_t index)
{
    if (index <= 0x1f)
    {
        // Exception
        exception_handler(index);
    }
    else if (index <= 0x20)
    {
        // PIC
    }
    else if (index <= 0x2f)
    {
        // Nothing
    }
    else if (index <= 0x7f)
    {
        // APIC
    }
    else if (index <= 0x80)
    {
        // Syscall
    }
    else if (index <= 0xee)
    {
        // More APIC
    }
    else if (index <= 0xef)
    {
         // local APIC timer
    }
    else
    {
        // SMP communication
    }
}

// This function initializes IDT table,
//		And loads the table to IDTR
void init_idt(void)
{
	int i = 0;
	for(; i <= 0x1f; i++)
	{
		// Exception

		INIT_INT_DESC(idt[i], KERNEL_CS_SEL);
		// Exception must be ONLY generated by CPU or Ring0
		// Actually the only IDT descriptor with DPL=3 is SYSCALL !!!
		idt[i].dpl = 0;
		idt[i].present = 1;
		SET_IDT_DESC_OFFSET(idt[i], raw_interrupt_handlers[i]);
	}
	for(; i <= 0x20; i++)
	{
		// PIC

		INIT_TRAP_DESC(idt[i], KERNEL_CS_SEL);
		// PIC IRQ INT must be ONLY generated by PIC or Ring0
		// Actually the only IDT descriptor with DPL=3 is SYSCALL !!!
		idt[i].dpl = 0;
		idt[i].present = 1;
		SET_IDT_DESC_OFFSET(idt[i], raw_interrupt_handlers[i]);
	}
	for(; i <= 0x7f; i++)
	{
		// either NOTHING or APIC

		// Not defined <=> Trap and Not present and DPL = 0
		INIT_TRAP_DESC(idt[i], KERNEL_CS_SEL);
		idt[i].dpl = 0;
		idt[i].present = 0;
		SET_IDT_DESC_OFFSET(idt[i], raw_interrupt_handlers[i]);
	}
	for(; i <= 0x80; i++)
	{
		// Syscall
		INIT_TRAP_DESC(idt[i], KERNEL_CS_SEL);
		idt[i].dpl = 3;
		idt[i].present = 1;
		SET_IDT_DESC_OFFSET(idt[i], raw_interrupt_handlers[i]);
	}
	for(; i <= 0xff; i++)
	{
		// More APIC, local APIC timer, OR SMP communication

		// Not defined <=> Trap and Not present and DPL = 0
		INIT_TRAP_DESC(idt[i], KERNEL_CS_SEL);
		idt[i].dpl = 0;
		idt[i].present = 0;
		SET_IDT_DESC_OFFSET(idt[i], raw_interrupt_handlers[i]);
	}
}
