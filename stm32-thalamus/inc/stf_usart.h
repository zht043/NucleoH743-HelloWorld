#ifndef __STF_USART_H
#define __STF_USART_H 


#include "stf_dependancy.h"


#ifdef HAL_UART_MODULE_ENABLED

#define USART_Default_TxTimeOut 60000 // 1 min
#define USART_Default_RxTimeOut 60000 // 1 min


#define USART_Default_Tx_BufferSize 3
#define USART_Default_Rx_BufferSize 50
#define Max_Num_USARTs 10 



namespace stf {
    class USART {
    private:
        UART_HandleTypeDef *huartx;

        // time-out default
        uint32_t tx_timeout = USART_Default_TxTimeOut;
        uint32_t rx_timeout = USART_Default_RxTimeOut;

        volatile periph_status tx_status = NotReady;
        volatile periph_status rx_status = NotReady;

        byte_t *tx_buffer;
        byte_t *rx_buffer;

        void hal_transmit(byte_t* bytes_ptr, uint16_t num_bytes, periph_mode mode);
        void hal_receive(byte_t* bytes_ptr, uint16_t num_bytes, periph_mode mode);

    public:

        // Constructors & Destructors
        USART(UART_HandleTypeDef *huartx, uint32_t tx_buffer_size = USART_Default_Tx_BufferSize, 
                                          uint32_t rx_buffer_size = USART_Default_Rx_BufferSize);
        ~USART();

        // Transmit & Receive
        void transmit(std::string& str, periph_mode mode = Polling) {
            char *cstr = (char*)str.c_str();
            hal_transmit((byte_t*)cstr, strlen(cstr), mode);
        }

        void transmit(char* str_ptr, periph_mode mode = Polling) {
            hal_transmit((byte_t*)str_ptr, strlen(str_ptr), mode);
        }
        void transmit(byte_t byte, periph_mode mode = Polling) {
            tx_buffer[0] = byte;
            hal_transmit(tx_buffer, 1, mode);
        }

                
        /* only support Polling mode if the return type is a string, 
        * this is due to c++ string constructor only does deep copy 
        * instead of shallow copy, can't copy a char array (rx_buffer) right away
        * when non-blocking receive is still in the middle of filling
        * content into the array
        * (non-blocking methods are {Interrupt, DMA})
        */
        std::string receive(uint16_t num_bytes) {
            hal_receive(rx_buffer, num_bytes, Polling);
            if(rx_status == Completed) 
                return std::string((char*)rx_buffer);
            else return ""; // if error occurs
        }

        void receive(char* buffer_ptr, uint16_t num_bytes, periph_mode mode = Polling) {
            hal_receive((byte_t*)buffer_ptr, num_bytes, mode);
        }

        /*only support Polling mode due to the peripheral needs 
         *to finish receiption before returning the results
         *it's inherently a blocking method
         */
        byte_t receive(void) {  
            hal_receive(rx_buffer, 1, Polling);
            if(rx_status == Completed) return rx_buffer[0];
            else return 0; // if error occurs
        }

        const std::string readWord(void); // polling mode
        const std::string readLine(void); // polling mode

        // Getters & Setters
        inline UART_HandleTypeDef *get_huartx(void) {  return huartx; }
        inline void set_tx_timeout(uint32_t t) { tx_timeout = t; }
        inline void set_rx_timeout(uint32_t t) { rx_timeout = t;}
        inline periph_status get_tx_status() { return tx_status; }
        inline periph_status get_rx_status() { return rx_status; }
        inline void set_tx_status(periph_status status) { tx_status = status; }
        inline void set_rx_status(periph_status status) { rx_status = status; } 
        inline byte_t* get_tx_buffer_ptr(void) {return tx_buffer;}                                                                                         
        inline byte_t* get_rx_buffer_ptr(void) {return rx_buffer;}

    };
}

/* Operators */ 

// polling mode print
template<typename T>
stf::USART& operator<<(stf::USART& obj, const T& output) {
    std::stringstream newStream;
    newStream << output;
    std::string str = newStream.str();
    obj.transmit(str, stf::Polling);
    return obj;
}

/* Callbacks */
__weak void usart_transmit_completed_interrupt_task(stf::USART* instance);
extern "C" void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart);

extern "C" void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
__weak void usart_receive_completed_interrupt_task(stf::USART* instance);




#endif

#endif 
