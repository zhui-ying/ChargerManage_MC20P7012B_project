
@nosvf @interrupt void SWI_ISR(void)
       	{
       	  
       	}

@nosvf @interrupt void INT1_ISR(void)
       	{
       	  INT1F=0;
       	  
       	}

@nosvf @interrupt void INT0_ISR(void)
       	{
       	  INT0F=0;
       	  
       	}

@nosvf @interrupt void T0INT_ISR(void)
       	{      	
       	     T0F=0;
       T0DATA=62;//125us                
         time_125us++;            
         if(time_125us>=81)
           {
               	time_125us=0;
                time_10ms++;
           } 
              

       	}

@nosvf @interrupt void PWMINT_ISR(void)
       	{
       	  
       	}
