#include <iostream>
#include <iomanip>
#include "thumbsim.hpp"

using namespace std;


/* Decode to find what type it is */
Thumb_Types decode (const ALL_Types);

/* Decodes for each type */
ALU_Ops decode (const ALU_Type);
DP_Ops decode (const DP_Type);
SP_Ops decode (const SP_Type);
LD_ST_Ops decode (const LD_ST_Type);
MISC_Ops decode (const MISC_Type);
int decode (const COND_Type);
int decode (const UNCOND_Type);
int decode (const LDM_Type);
int decode (const STM_Type);
int decode (const LDRL_Type);
int decode (const ADD_SP_Type);

Thumb_Types decode (const ALL_Types data) {
   if (data.type.alu.instr.class_type.type_check == ALU_TYPE) {
      return ALU;
   }
   else if (data.type.dp.instr.class_type.type_check == DP_TYPE) {
      return DP;
   }
   else if (data.type.sp.instr.class_type.type_check == SP_TYPE) {
      return SPECIAL;
   }
   else if (data.type.uncond.instr.class_type.type_check == UNCOND_TYPE) {
      return UNCOND;
   }
   else if (data.type.misc.instr.class_type.type_check == MISC_TYPE) {
      return MISC;
   }
   else if (data.type.cond.instr.class_type.type_check == COND_TYPE) {
      return COND;
   }
   else if (data.type.ldm.instr.class_type.type_check == LDM_TYPE) {
      return LDM;
   }
   else if (data.type.stm.instr.class_type.type_check == STM_TYPE) {
      return STM;
   }
   else if (data.type.ldrl.instr.class_type.type_check == LDRL_TYPE) {
      return LDRL;
   }
   else if (data.type.addsp.instr.class_type.type_check == ADD_SP_TYPE) {
      return ADD_SP;
   }
   else if (data.type.bl.instr.class_type.type_check == BL_TYPE) {
      return BL;
   }
   else {
      if (data.type.ld_st.instr.class_type.opA == LD_ST_REG_OPA) {
      }
      else if (data.type.ld_st.instr.class_type.opA == LD_ST_IMM_OPA) {
      }
      else if (data.type.ld_st.instr.class_type.opA == LD_ST_IMMB_OPA) {
      }
      else if (data.type.ld_st.instr.class_type.opA == LD_ST_IMMH_OPA) {
      }
      else if (data.type.ld_st.instr.class_type.opA == LD_ST_IMMSP_OPA) {
      }
      else {
         cout << "NO TYPE FOUND" << endl;
         return ERROR_TYPE;
      }
      return LD_ST;
   }
}

ALU_Ops decode (const ALU_Type data) {
   if (opts.instrs) { 
	   if (data.instr.lsli.op == ALU_LSLI_OP) {
			 cout << "lsls r";
			 cout << data.instr.lsli.rd;
			 cout << ", r";
			 cout << data.instr.lsli.rm;
			 cout << ", #";
			 cout << data.instr.lsli.imm << endl;
			 return ALU_LSLI;
	   }
	   else if (data.instr.lsri.op == ALU_LSRI_OP) {
			 cout << "lsr r";
			 cout << data.instr.lsri.rd;
			 cout << ", r";
			 cout << data.instr.lsri.rm;
			 cout << ", #";
			 cout << data.instr.lsri.imm << endl;
			 return ALU_LSRI;
	   }
	   else if (data.instr.asri.op == ALU_ASRI_OP) {
			 cout << "asrs r";
			 cout << data.instr.asri.rd;
			 cout << ", r";
			 cout << data.instr.asri.rm;
			 cout << ", #";
			 cout << data.instr.asri.imm << endl;
			 return ALU_ASRI;
	   }
	   else if (data.instr.addr.op == ALU_ADDR_OP) {
		  if (opts.instrs) { 
			 cout << "adds r" << data.instr.addr.rd  << ", r" << data.instr.addr.rn << ", r" << data.instr.addr.rm << endl;
		  }
		  return ALU_ADDR;
	   }
	   else if (data.instr.subr.op == ALU_SUBR_OP) {
			 cout << "sub r";
			 cout << data.instr.subr.rd;
			 cout << ", r";
			 cout << data.instr.subr.rn;
			 cout << ", r";
			 cout << data.instr.subr.rm << endl;
			 return ALU_SUBR;
	   }
	   else if (data.instr.add3i.op == ALU_ADD3I_OP) {
		  if (opts.instrs) { 
			 cout << "adds r" << data.instr.add3i.rd << ", r" << data.instr.add3i.rn << ", #" << data.instr.add3i.imm << endl;
		  }
		  return ALU_ADD3I;
	   }
	   else if (data.instr.sub3i.op == ALU_SUB3I_OP) {
			 cout << "subs r";
			 cout << data.instr.sub3i.rd;
			 cout << ", r";
			 cout << data.instr.sub3i.rn;
			 cout << ", #";
			 cout << data.instr.sub3i.imm << endl;
			 return ALU_SUB3I;
	   }
	   else if (data.instr.add8i.op == ALU_ADD8I_OP) {
		  if (opts.instrs) { 
			 cout << "adds r" << data.instr.add8i.rdn << ", #" << data.instr.add8i.imm << endl;
		  }
		  return ALU_ADD8I;
	   }
	   else if (data.instr.sub8i.op == ALU_SUB8I_OP) {
			 cout << "subs r";
			 cout << data.instr.sub8i.rdn;
			 cout << ", #";
			 cout << data.instr.sub3i.imm << endl;
			 return ALU_SUB8I;
	   }
	   else if (data.instr.cmp.op == ALU_CMP_OP) { 
		  if (opts.instrs) { 
			 cout << "cmp r" << data.instr.cmp.rdn << ", #" << data.instr.cmp.imm << endl;
		  }
		  return ALU_CMP;
	   }
	   else if (data.instr.mov.op == ALU_MOV_OP) { 
		  if (opts.instrs) { 
			 cout << "movs r" << data.instr.mov.rdn << ", #" << setbase(10) << data.instr.mov.imm << endl;
		  }
		  return ALU_MOV;
	   }
    }
}
DP_Ops decode (const DP_Type data) {
	if (opts.instrs) { 
	  if (data.instr.DP_Instr.op == DP_AND_OP) {
         cout << "ands ";
		 cout << "r";
         cout << data.instr.DP_Instr.rdn;
         cout << ", r";
         cout << data.instr.DP_Instr.rm << endl;
		 return DP_AND;
	  }
       else if (data.instr.DP_Instr.op == DP_EOR_OP) {
         cout << "eors ";
		 cout << "r";
         cout << data.instr.DP_Instr.rdn;
         cout << ", r";
         cout << data.instr.DP_Instr.rm << endl;	
		 return DP_EOR;
      }
       else if (data.instr.DP_Instr.op == DP_LSL_OP) {
         cout << "lsls ";
		 cout << "r";
         cout << data.instr.DP_Instr.rdn;
         cout << ", r";
         cout << data.instr.DP_Instr.rm << endl;	
		 return DP_LSL;
      }
       else if (data.instr.DP_Instr.op == DP_LSR_OP) {
         cout << "lsrs ";
		 cout << "r";
         cout << data.instr.DP_Instr.rdn;
         cout << ", r";
         cout << data.instr.DP_Instr.rm << endl;
		 return DP_LSR;
      }
       else if (data.instr.DP_Instr.op == DP_ASR_OP) {
         cout << "asrs ";
		 cout << "r";
         cout << data.instr.DP_Instr.rdn;
         cout << ", r";
         cout << data.instr.DP_Instr.rm << endl;	
		 return DP_ASR;
      }
       else if (data.instr.DP_Instr.op == DP_ADC_OP) {
         cout << "adcs ";
		 cout << "r";
         cout << data.instr.DP_Instr.rdn;
         cout << ", r";
         cout << data.instr.DP_Instr.rm << endl;	
		 return DP_ADC;
      }
       else if (data.instr.DP_Instr.op == DP_SBC_OP) {
         cout << "sbcs ";
		 cout << "r";
         cout << data.instr.DP_Instr.rdn;
         cout << ", r";
         cout << data.instr.DP_Instr.rm << endl;	
		 return DP_SBC;
      }
       else if (data.instr.DP_Instr.op == DP_ROR_OP) {
         cout << "rors ";
		 cout << "r";
         cout << data.instr.DP_Instr.rdn;
         cout << ", r";
         cout << data.instr.DP_Instr.rm << endl;	
		 return DP_ROR;
      }
       else if (data.instr.DP_Instr.op == DP_TST_OP) {
         cout << "tst ";
		 cout << "r";
         cout << data.instr.DP_Instr.rdn;
         cout << ", r";
         cout << data.instr.DP_Instr.rm << endl;	
		 return DP_TST;
      }
       else if (data.instr.DP_Instr.op == DP_RSB_OP) {
         cout << "rsbs ";
		 cout << "r";
         cout << data.instr.DP_Instr.rdn;
         cout << ", r";
         cout << data.instr.DP_Instr.rm << endl;	
		 return DP_RSB;
      }
       else if (data.instr.DP_Instr.op == DP_CMP_OP) {
         cout << "cmp ";
		 cout << "r";
         cout << data.instr.DP_Instr.rdn;
         cout << ", r";
         cout << data.instr.DP_Instr.rm << endl;	
		 return DP_CMP;
      }
       else if (data.instr.DP_Instr.op == DP_CMN_OP) {
         cout << "cmn ";
		 cout << "r";
         cout << data.instr.DP_Instr.rdn;
         cout << ", r";
         cout << data.instr.DP_Instr.rm << endl;	
		 return DP_CMN;
      }
       else if (data.instr.DP_Instr.op == DP_ORR_OP) {
         cout << "orrs ";
		 cout << "r";
         cout << data.instr.DP_Instr.rdn;
         cout << ", r";
         cout << data.instr.DP_Instr.rm << endl;		
		 return DP_ORR;
      }
       else if (data.instr.DP_Instr.op == DP_MUL_OP) {
         cout << "muls ";
		 cout << "r";
         cout << data.instr.DP_Instr.rdn;
         cout << ", r";
         cout << data.instr.DP_Instr.rm << endl;	
		 return DP_MUL;
      }
       else if (data.instr.DP_Instr.op == DP_BIC_OP) {
         cout << "bics ";
		 cout << "r";
         cout << data.instr.DP_Instr.rdn;
         cout << ", r";
         cout << data.instr.DP_Instr.rm << endl;	
		 return DP_BIC;
      }
       else if (data.instr.DP_Instr.op == DP_MVN_OP) {
         cout << "mvns ";
		 cout << "r";
         cout << data.instr.DP_Instr.rdn;
         cout << ", r";
         cout << data.instr.DP_Instr.rm << endl;		 
		 return DP_MVN;
      }
	}
}

SP_Ops decode (const SP_Type data) {
   if (data.instr.mov.op == 2) {
      if (opts.instrs) { 
         cout << "mov";
         if (data.instr.mov.d == 1 && data.instr.mov.rd == 5) {
            cout << " sp, r" << data.instr.mov.rm << endl;
         }
         else {
			if(data.instr.mov.d == 1){
				cout << " r" << data.instr.mov.rd + 8 << ", r" << data.instr.mov.rm << endl;
			}
			else{
				cout << " r" << data.instr.mov.rd << ", r" << data.instr.mov.rm << endl;
			}
         }
      }
      return SP_MOV;
   }
   else if( data.instr.add.op == 0) {
      if (opts.instrs) {
	     cout << "add";
	     if (data.instr.add.d == 1 && data.instr.add.rd == 0b101 || data.instr.add.rm == 0b1101) {
			cout << " sp, sp, r" << data.instr.add.rm << endl;
         }
         else {
            cout << " r" << data.instr.add.rd << ", r" << data.instr.add.rd << ", r" << data.instr.add.rm << endl;
         }
	  }
	  return SP_ADD;
   }
   else {
      if (opts.instrs) { 
         cout << "nop" << endl;
      }
   }

}
LD_ST_Ops decode (const LD_ST_Type data) {
   if (opts.instrs) {
	   if (data.instr.class_type.opA == LD_ST_REG_OPA) {
			if (data.instr.class_type.opB == LD_ST_OPB_STR) {
				cout << "str r";
				data.instr.ld_st_reg.rm;
				cout << ", [r";
				cout << data.instr.ld_st_reg.rn;
				cout << ", r";
				cout << data.instr.ld_st_reg.rt;
				cout << "]" << endl;
				return STRR;
			}
			else if (data.instr.class_type.opB == LD_ST_OPB_STRH) {
				cout << "strh r";
				//NEED TO DO
				return STRHR;
			}
			else if (data.instr.class_type.opB == LD_ST_OPB_STRB) {
				cout << "strb r";
				//NEED TO DO
				return STRBR;
			}else if (data.instr.class_type.opB == LD_ST_OPB_LDRSB) {
				cout << "ldrsb r";
				//NEED TO DO
				return LDRSBR;
			}
			else if (data.instr.class_type.opB == LD_ST_OPB_LDR) {
				cout << "ldr r";
				data.instr.ld_st_reg.rm;
				cout << ", [r";
				cout << data.instr.ld_st_reg.rn;
				cout << ", r";
				cout << data.instr.ld_st_reg.rt;
				cout << "]" << endl;
				return LDRR;
			}
			else if (data.instr.class_type.opB == LD_ST_OPB_LDRH) {
				cout << "ldrh r";
				//NEED TO DO
				return LDRHR;
			}
			else if (data.instr.class_type.opB == LD_ST_OPB_LDRB) {
				cout << "ldrb r";
				//NEED TO DO
				return LDRBR;
			}
			else if (data.instr.class_type.opB == LD_ST_OPB_LDRSH) {
				cout << "ldrsh r";
				//NEED TO DO
				return LDRSHR;
			}
	   }
	   else if (data.instr.class_type.opA == LD_ST_IMM_OPA) {
		  if (data.instr.class_type.opB == LD_ST_OPB_STR) {
			 if (opts.instrs) { 
				cout << "str r" << data.instr.ld_st_imm.rt << ", [r" << data.instr.ld_st_imm.rn << ", #" << setbase(10) << (data.instr.ld_st_imm.imm*4) << "]" << endl;
			 }
			 return STRI;
		  }
		  else if (data.instr.class_type.opB == LD_ST_OPB_LDR) {
			 if (opts.instrs) { 
				cout << "ldr r" << data.instr.ld_st_imm.rt << ", [r" << data.instr.ld_st_imm.rn << ", #" << setbase(10) << (data.instr.ld_st_imm.imm*4) << "]" << endl;
			 }
			 return LDRI;
		  }
	   }
	   else if (data.instr.class_type.opA == LD_ST_IMMB_OPA) {
			if (data.instr.class_type.opB & 4 == 0) {
				cout << "strb r";
				cout << data.instr.ld_st_imm.rn;
				cout << ", [r";
				cout << data.instr.ld_st_imm.rt;
				cout << ", #";
				cout <<  setbase(10) << (static_cast<unsigned int>(data.instr.ld_st_imm.imm)<< 2);
				cout << "]" << endl;
				return STRBI;
			}
			else if (data.instr.class_type.opB & 4) {
				cout << "ldrb r";
				cout << data.instr.ld_st_imm.rn;
				cout << ", [r";
				cout << data.instr.ld_st_imm.rt;
				cout << ", #";
				cout <<  setbase(10) << (static_cast<unsigned int>(data.instr.ld_st_imm.imm)<< 2);
				cout << "]" << endl;
				return LDRBI;
			}
	   }
	   else if (data.instr.class_type.opA == LD_ST_IMMH_OPA) {
			if (data.instr.class_type.opB & 4 == 0) {
				cout << "strh r";
				cout << data.instr.ld_st_imm.rn;
				cout << ", [r";
				cout << data.instr.ld_st_imm.rt;
				cout << ", #";
				cout <<  setbase(10) << (static_cast<unsigned int>(data.instr.ld_st_imm.imm)<< 2);
				cout << "]" << endl;
				return STRHI;
			}
			else if (data.instr.class_type.opB & 4) {
				cout << "ldrh r";
				cout << data.instr.ld_st_imm.rn;
				cout << ", [r";
				cout << data.instr.ld_st_imm.rt;
				cout << ", #";
				cout <<  setbase(10) << (static_cast<unsigned int>(data.instr.ld_st_imm.imm)<< 2);
				cout << "]" << endl;
				return LDRHI;
			}
	   }
	   else if (data.instr.class_type.opA == LD_ST_IMMSP_OPA) {
			if (data.instr.class_type.opB & 4 == 0) {
				cout << "str r";
				cout << data.instr.ld_st_sp.rt;
				cout << ", [sp, #";
				cout <<  setbase(10) << (static_cast<unsigned int>(data.instr.ld_st_sp.imm)<< 2);
				cout << "]" << endl;
				return STRSP;
			}
			else if (data.instr.class_type.opB & 4) {
				cout << "ldr r";
				cout << data.instr.ld_st_sp.rt;
				cout << ", [sp, #";
				cout <<  setbase(10) << (static_cast<unsigned int>(data.instr.ld_st_sp.imm)<< 2);
				cout << "]" << endl;
				return LDRSP;
			}
	   }
	}
}
MISC_Ops decode (const MISC_Type data) {
   if (data.instr.push.op == MISC_PUSH_OP) {
      if (opts.instrs) { 
       bool multiple = FALSE;
       cout << "push ";
       cout << "{";
       if (data.instr.push.reg_list & 1) {
         cout << "r0";
         multiple = TRUE;
       }
       if (data.instr.push.reg_list & 2) {
         if (multiple)
           cout << ", ";
         cout << "r1";
         multiple = TRUE;
       }
       if (data.instr.push.reg_list & 4) {
         if (multiple)
           cout << ", ";
         cout << "r2";
         multiple = TRUE;
       }
       if (data.instr.push.reg_list & 8) {
         if (multiple)
           cout << ", ";
         cout << "r3";
         multiple = TRUE;
       }
       if (data.instr.push.reg_list & 16) {
         if (multiple)
           cout << ", ";
         cout << "r4";
         multiple = TRUE;
       }
       if (data.instr.push.reg_list & 32) {
         if (multiple)
           cout << ", ";
         cout << "r5";
         multiple = TRUE;
       }
       if (data.instr.push.reg_list & 64) {
         if (multiple)
           cout << ", ";
         cout << "r6";
         multiple = TRUE;
       }
       if (data.instr.push.reg_list & 128) {
         if (multiple)
           cout << ", ";
         cout << "r7";
         multiple = TRUE;
       }
       if (data.instr.push.m) {
         if (multiple)
           cout << ", ";
         cout << "lr";
       }
       cout << "}" << endl;
      }
      return MISC_PUSH;
   }
   else if (data.instr.pop.op == MISC_POP_OP) {
     if (opts.instrs) { 
       bool multiple = FALSE;
       cout << "pop ";
       cout << "{";
       if (data.instr.pop.reg_list & 1) {
         cout << "r0";
         multiple = TRUE;
       }
       if (data.instr.pop.reg_list & 2) {
         if (multiple)
           cout << ", ";
         cout << "r1";
         multiple = TRUE;
       }
       if (data.instr.pop.reg_list & 4) {
         if (multiple)
           cout << ", ";
         cout << "r2";
         multiple = TRUE;
       }
       if (data.instr.pop.reg_list & 8) {
         if (multiple)
           cout << ", ";
         cout << "r3";
         multiple = TRUE;
       }
       if (data.instr.pop.reg_list & 16) {
         if (multiple)
           cout << ", ";
         cout << "r4";
         multiple = TRUE;
       }
       if (data.instr.pop.reg_list & 32) {
         if (multiple)
           cout << ", ";
         cout << "r5";
         multiple = TRUE;
       }
       if (data.instr.pop.reg_list & 64) {
         if (multiple)
           cout << ", ";
         cout << "r6";
         multiple = TRUE;
       }
       if (data.instr.pop.reg_list & 128) {
         if (multiple)
           cout << ", ";
         cout << "r7";
         multiple = TRUE;
       }
       if (data.instr.pop.m) {
         if (multiple)
           cout << ", ";
         cout << "pc";
       }
       cout << "}" << endl;
     }
      return MISC_POP;
   }
   else if (data.instr.sub.op == MISC_SUB_OP) {
      if (opts.instrs) { 
         cout << "sub sp, #";
         cout << setbase(10) << (static_cast<unsigned int>(data.instr.sub.imm)<< 2) << endl;
      } 
      return MISC_SUB;
   }
   else if (data.instr.add.op == MISC_ADD_OP) {
      if (opts.instrs) { 
         cout << "add sp, #";
         cout << setbase(10) << (static_cast<unsigned int>(data.instr.add.imm)<< 2) << endl;
      } 
      return MISC_ADD;
   }
   else if (data.instr.class_type.type_check == ADD_SP8I_TYPE) {
      if (opts.instrs) { 
         cout << "add sp, #" << setbase(10) << (data.instr.add.imm<<2)<< endl;
      }
      return MISC_ADD;
   }

}

int decode (const COND_Type data) {
   if (opts.instrs) { 
      cout << "b";
      printCond(data.instr.b.cond);
      cout << " 0x" << hex << data.instr.b.imm << endl;
   }
}

int decode (const UNCOND_Type data) {
   if (opts.instrs) { 
      cout << "b 0x" << hex << data.instr.b.imm << endl;
   }
}

int decode (const LDM_Type data) {
	if (opts.instrs) { 
       bool multiple = FALSE;
       cout << "ldm r" << data.instr.ldm.rn << "!, ";
       cout << "{";
       if (data.instr.ldm.reg_list & 1) {
         cout << "r0";
         multiple = TRUE;
       }
       if (data.instr.ldm.reg_list & 2) {
         if (multiple)
           cout << ", ";
         cout << "r1";
         multiple = TRUE;
       }
       if (data.instr.ldm.reg_list & 4) {
         if (multiple)
           cout << ", ";
         cout << "r2";
         multiple = TRUE;
       }
       if (data.instr.ldm.reg_list & 8) {
         if (multiple)
           cout << ", ";
         cout << "r3";
         multiple = TRUE;
       }
       if (data.instr.ldm.reg_list & 16) {
         if (multiple)
           cout << ", ";
         cout << "r4";
         multiple = TRUE;
       }
       if (data.instr.ldm.reg_list & 32) {
         if (multiple)
           cout << ", ";
         cout << "r5";
         multiple = TRUE;
       }
       if (data.instr.ldm.reg_list & 64) {
         if (multiple)
           cout << ", ";
         cout << "r6";
         multiple = TRUE;
       }
       if (data.instr.ldm.reg_list & 128) {
         if (multiple)
           cout << ", ";
         cout << "r7";
         multiple = TRUE;
       }
       cout << "}" << endl;
	}
}

int decode (const STM_Type data) {
    if (opts.instrs) { 
       bool multiple = FALSE;
       cout << "stm r" << data.instr.stm.rn << "!, ";
       cout << "{";
       if (data.instr.stm.reg_list & 1) {
         cout << "r0";
         multiple = TRUE;
       }
       if (data.instr.stm.reg_list & 2) {
         if (multiple)
           cout << ", ";
         cout << "r1";
         multiple = TRUE;
       }
       if (data.instr.stm.reg_list & 4) {
         if (multiple)
           cout << ", ";
         cout << "r2";
         multiple = TRUE;
       }
       if (data.instr.stm.reg_list & 8) {
         if (multiple)
           cout << ", ";
         cout << "r3";
         multiple = TRUE;
       }
       if (data.instr.stm.reg_list & 16) {
         if (multiple)
           cout << ", ";
         cout << "r4";
         multiple = TRUE;
       }
       if (data.instr.stm.reg_list & 32) {
         if (multiple)
           cout << ", ";
         cout << "r5";
         multiple = TRUE;
       }
       if (data.instr.stm.reg_list & 64) {
         if (multiple)
           cout << ", ";
         cout << "r6";
         multiple = TRUE;
       }
       if (data.instr.stm.reg_list & 128) {
         if (multiple)
           cout << ", ";
         cout << "r7";
         multiple = TRUE;
       }
       cout << "}" << endl;
	}
}

int decode (const LDRL_Type data) {
   if (opts.instrs) { 
      cout << "ldr r" << data.instr.ldrl.rt << ", [pc, #" << setbase(10) <<  data.instr.ldrl.imm * 4 << "]" << endl;
    }
}

int decode (const ADD_SP_Type data) {
   if (opts.instrs) { 
      cout << "add r" << data.instr.add.rd << ", sp, #" << data.instr.add.imm * 4 << endl;
   }
}

// Add this under:
/* Decodes for each type */
BL_Ops decode (const BL_Type);

// Finally add this new decode function
// It doesn't print correctly, sorry. 
// Feel free to edit.
BL_Ops decode (const BL_Type data) {
  if (opts.instrs) { 
    cout << "bl 0x" << hex << data.instr.bl_upper.imm10 << endl;
  }
  return BL_UPPER;
}