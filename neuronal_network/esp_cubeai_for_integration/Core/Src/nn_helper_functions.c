///*
// * nn_helper_functions.c
// *
// *  Created on: May 16, 2024
// *      Author: STM32 Microelectronics
// */
//
///* -----------------------------------------------------------------------------
// * AI-related functions
// * -----------------------------------------------------------------------------
// */
//
//#include "nn_helper_functions.h"
//
//#include <stdio.h>
//#include <stdarg.h>
//
//void SENSING1_PRINTF(const char *format, ...) {
//    va_list args;
//    va_start(args, format);
//    vprintf(format, args);
//    va_end(args);
//}
//
//
//void aiLogErr(const ai_error err, const char *fct)
//{
//    if (fct)
//        SENSING1_PRINTF("E: AI error (%s) - type=%d code=%d\r\n", fct,
//                err.type, err.code);
//    else
//        SENSING1_PRINTF("E: AI error - type=%d code=%d\r\n", err.type, err.code);
//}
//
//
//void aiPrintLayoutBuffer(const char *msg, int idx,
//        const ai_buffer* buffer)
//{
//    uint32_t type_id = AI_BUFFER_FMT_GET_TYPE(buffer->format);
//    SENSING1_PRINTF("%s ",msg);
//    if (type_id == AI_BUFFER_FMT_TYPE_Q) {
//        SENSING1_PRINTF(" %s%d,",
//        		AI_BUFFER_FMT_GET_SIGN(buffer->format)?"s":"u",
//                (int)AI_BUFFER_FMT_GET_BITS(buffer->format));
//        if (AI_BUFFER_META_INFO_INTQ(buffer->meta_info)) {
//            SENSING1_PRINTF(" scale=%f, zero=%d,",AI_BUFFER_META_INFO_INTQ_GET_SCALE(buffer->meta_info, 0), AI_BUFFER_META_INFO_INTQ_GET_ZEROPOINT(buffer->meta_info, 0));
//    	} else {
//    		SENSING1_PRINTF("Q%d.%d,",
//    				(int)AI_BUFFER_FMT_GET_BITS(buffer->format)
//					- ((int)AI_BUFFER_FMT_GET_FBITS(buffer->format) +
//					(int)AI_BUFFER_FMT_GET_SIGN(buffer->format)),
//					AI_BUFFER_FMT_GET_FBITS(buffer->format));
//    	}
//    }
//    else if (type_id == AI_BUFFER_FMT_TYPE_FLOAT)
//        SENSING1_PRINTF(" float%d,",
//                (int)AI_BUFFER_FMT_GET_BITS(buffer->format));
//    else
//        SENSING1_PRINTF("NONE");
//    SENSING1_PRINTF(" %ld bytes\r\n",
//    		AI_BUFFER_BYTE_SIZE(AI_BUFFER_SIZE(buffer), buffer->format));
//}
//
//void aiPrintNetworkInfo(const ai_network_report* report)
//{
//    int i;
//    SENSING1_PRINTF("Network configuration...\r\n");
//    SENSING1_PRINTF(" Model name         : %s\r\n", report->model_name);
//    SENSING1_PRINTF(" Model signature    : %s\r\n", report->model_signature);
//    SENSING1_PRINTF(" Model datetime     : %s\r\n", report->model_datetime);
//    SENSING1_PRINTF(" Compile datetime   : %s\r\n", report->compile_datetime);
//    SENSING1_PRINTF(" Runtime revision   : %d.%d.%d\r\n",
//            report->runtime_version.major,
//            report->runtime_version.minor,
//            report->runtime_version.micro);
//    SENSING1_PRINTF(" Tool revision      : %s (%d.%d.%d)\r\n", report->tool_revision,
//            report->tool_version.major,
//            report->tool_version.minor,
//            report->tool_version.micro);
//    SENSING1_PRINTF("Network info...\r\n");
//    SENSING1_PRINTF("  nodes             : %ld\r\n", report->n_nodes);
//    SENSING1_PRINTF("  complexity        : %ld MACC\r\n", report->n_macc);
//    SENSING1_PRINTF("  activation        : %ld bytes\r\n",
//            AI_BUFFER_SIZE(&report->activations));
//    SENSING1_PRINTF("  params            : %ld bytes\r\n",
//            AI_BUFFER_SIZE(&report->params));
//    SENSING1_PRINTF("  inputs/outputs    : %u/%u\r\n", report->n_inputs,
//            report->n_outputs);
//    for (i=0; i<report->n_inputs; i++)
//        aiPrintLayoutBuffer("   I", i, &report->inputs[i]);
//    for (i=0; i<report->n_outputs; i++)
//        aiPrintLayoutBuffer("   O", i, &report->outputs[i]);
//}
//
///* -----------------------------------------------------------------------------
// * AI-related functions 2
// * -----------------------------------------------------------------------------
// */
//int aiConvertInputFloat_2_Int8(ai_float *In_f32, ai_i8 *Out_int8)
//{
//  if( AI_HANDLE_NULL == audio_net_ctx.handle)
//  {
//      SENSING1_PRINTF("E: network handle is NULL\r\n");
//      return -1;
//  }
//  ai_buffer * bufferPtr   = &(audio_net_ctx.report.inputs[0]);
//  ai_buffer_format format = bufferPtr->format;
//  int size  = AI_BUFFER_SIZE(bufferPtr);
//  ai_float scale ;
//  int zero_point ;
//
//  if (AI_BUFFER_FMT_TYPE_Q != AI_BUFFER_FMT_GET_TYPE(format) &&\
//    ! AI_BUFFER_FMT_GET_SIGN(format) &&\
//    8 != AI_BUFFER_FMT_GET_BITS(format))
//  {
//      SENSING1_PRINTF("E: expected signed integer 8 bits\r\n");
//      return -1;
//  }
//  if (AI_BUFFER_META_INFO_INTQ(bufferPtr->meta_info)) {
//      scale = AI_BUFFER_META_INFO_INTQ_GET_SCALE(bufferPtr->meta_info, 0);
//      if (scale != 0.0F)
//      {
//         scale= 1.0F/scale ;
//      }
//      else
//      {
//        SENSING1_PRINTF("E: division by zero\r\n");
//        return -1;
//      }
//      zero_point = AI_BUFFER_META_INFO_INTQ_GET_ZEROPOINT(bufferPtr->meta_info, 0);
//  } else {
//      SENSING1_PRINTF("E: no meta info\r\n");
//      return -1;
//  }
//
//  for (int i = 0; i < size ; i++)
//  {
//    Out_int8[i] = __SSAT((int32_t) roundf((float)zero_point + In_f32[i]*scale), 8);
//  }
//  return 0;
//}
//
//int aiConvertOutputInt8_2_Float(ai_i8 *In_int8, ai_float *Out_f32)
//{
//  if( AI_HANDLE_NULL == audio_net_ctx.handle)
//  {
//      SENSING1_PRINTF("E: network handle is NULL\r\n");
//      return -1;
//  }
//  ai_buffer * bufferPtr   = &(audio_net_ctx.report.outputs[0]);
//  ai_buffer_format format = bufferPtr->format;
//  int size  = AI_BUFFER_SIZE(bufferPtr);
//  ai_float scale ;
//  int zero_point ;
//
//  if (AI_BUFFER_FMT_TYPE_Q != AI_BUFFER_FMT_GET_TYPE(format) &&\
//    ! AI_BUFFER_FMT_GET_SIGN(format) &&\
//    8 != AI_BUFFER_FMT_GET_BITS(format))
//  {
//      SENSING1_PRINTF("E: expected signed integer 8 bits\r\n");
//      return -1;
//  }
//  if (AI_BUFFER_META_INFO_INTQ(bufferPtr->meta_info)) {
//      scale = AI_BUFFER_META_INFO_INTQ_GET_SCALE(bufferPtr->meta_info, 0);
//      zero_point = AI_BUFFER_META_INFO_INTQ_GET_ZEROPOINT(bufferPtr->meta_info, 0);
//  } else {
//      SENSING1_PRINTF("E: no meta info\r\n");
//      return -1;
//  }
//
//  for (uint32_t i = 0; i < size ; i++)
//  {
//    Out_f32[i] = scale * ((ai_float)(In_int8[i]) - zero_point);
//  }
//  return 0;
//}
//
