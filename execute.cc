#include "thumbsim.hpp"
#define PC_REG 15
#define LR_REG 14
#define SP_REG 13
#define PC rf[PC_REG]
#define LR rf[LR_REG]
#define SP rf[SP_REG]

unsigned int signExtend16to32ui(short i) {
  return static_cast<unsigned int>(static_cast<int>(i));
}

unsigned int signExtend8to32ui(char i) {
  return static_cast<unsigned int>(static_cast<int>(i));
}

ASPR flags;

//All your base are belong to us!

// You're given the code for evaluating BEQ, 
// and you'll need to fill in the rest of these.
// See Page 99 of the armv6 manual
static int checkCondition(unsigned short cond) {
  switch(cond) {
    case EQ:
      if (flags.Z == 1) {
        return TRUE;
      }
      break;
    case NE:
      if (flags.Z == 0) {
        return TRUE;
      }
      break;
    case CS:
      if (flags.C == 1) {
        return TRUE;
      }
      break;
    case CC:
      if (flags.C == 0) {
        return TRUE;
      }
      break;
    case MI:
      if (flags.N == 1) {
        return TRUE;
      }
      break;
    case PL:
      if (flags.N == 0) {
        return TRUE;
      }
      break;
    case VS:
      if (flags.V == 1) {
        return TRUE;
      }
      break;
    case VC:
      if (flags.V == 0) {
        return TRUE;
      }
      break;
    case HI:
      if (flags.C == 1 && flags.Z == 0) {
        return TRUE;
      }
      break;
    case LS:
      if (flags.C == 0 || flags.Z == 1) {
        return TRUE;
      }
      break;
    case GE:
      if (flags.N == flags.V) {
        return TRUE;
      }
      break;
    case LT:
      if (flags.N != flags.V) {
        return TRUE;
      }
      break;
    case GT:
      if (flags.Z == 0 && flags.N == flags.V) {
        return TRUE;
      }
      break;
    case LE:
      if (flags.Z == 1 || flags.N != flags.V) {
        return TRUE;
      }
      break;
    case AL:
      return TRUE;
      break;
  }
  return FALSE;
}

  void setCarryOverflow (int num1, int num2, OFType oftype) {
	  switch (oftype) {
		case OF_ADD:
		  if (((unsigned long long int)num1 + (unsigned long long int)num2) ==
			  ((unsigned int)num1 + (unsigned int)num2)) {
			flags.C = 0;
		  }
		  else {
			flags.C = 1;
		  }
		  if (((long long int)num1 + (long long int)num2) ==
			  ((int)num1 + (int)num2)) {
			flags.V = 0;
		  }
		  else {
			flags.V = 1;
		  }
		  break;
		case OF_SUB:
		  if (((unsigned long long int)num1 - (unsigned long long int)num2) ==
			  ((unsigned int)num1 - (unsigned int)num2)) {
			flags.C = 0;
		  }
		  else {
			flags.C = 1;
		  }
		  if (((num1==0) && (num2==0)) || 
			  (((long long int)num1 - (long long int)num2) ==
			  ((int)num1 - (int)num2))) {
			flags.V = 0;
		  }
		  else {
			flags.V = 1;
		  }
		  break;
		case OF_SHIFT:
		  // C flag unaffected for shifts by zero
		  if (num2 != 0) {
			if (((unsigned long long int)num1 << (unsigned long long int)num2) ==
				((unsigned int)num1 << (unsigned int)num2)) {
			  flags.C = 0;
			}
			else {
			  flags.C = 1;
			}
		  }
		  // Shift doesn't set overflow
		  break;
		default:
		  cerr << "Bad OverFlow Type encountered." << __LINE__ << __FILE__ << endl;
		  exit(1);
	  }
  }

void execute() {
	/*cout << "rf0:" << rf[0] << " 1:" << rf[1] << " 2:" << rf[2] << " 3:" << rf[3] << 
	        " 4:" << rf[4] << " 5:" << rf[5] << " 6:" << rf[6] << " 7:" << rf[7] <<
			" sp:" << SP << " lr:" << LR << " pc:" << PC << endl;*/

  Data16 instr = imem[PC];
  Thumb_Types itype;
  unsigned int pctarget = PC + 2;
  unsigned int addr;
  int diff, BitCount, bit;

  /* Convert instruction to correct type */
  ALU_Type alu(instr);
  SP_Type sp(instr);
  DP_Type dp(instr);
  LD_ST_Type ld_st(instr);
  MISC_Type misc(instr);
  COND_Type cond(instr);
  UNCOND_Type uncond(instr);
  LDM_Type ldm(instr);
  STM_Type stm(instr);
  LDRL_Type ldrl(instr);
  ADD_SP_Type addsp(instr);
  BL_Type blupper(instr);
  
  ALU_Ops add_ops;
  DP_Ops dp_ops;
  SP_Ops sp_ops;
  LD_ST_Ops ldst_ops;
  MISC_Ops misc_ops;
  BL_Ops bl_ops;

  rf.write(PC_REG, pctarget);

  itype = decode(ALL_Types(instr));
  int tempMov = 0;
  switch(itype) {
    case ALU:
      add_ops = decode(alu);
      switch(add_ops) {
        case ALU_LSLI:
		  setCarryOverflow(rf[alu.instr.lsli.rd], rf[alu.instr.lsli.rm],OF_SHIFT);
          rf.write(alu.instr.lsli.rd, rf[alu.instr.lsli.rm] << alu.instr.lsli.imm);		  
		  if(rf[alu.instr.lsli.rd] & 1 << 31){
			flags.N = 1;
		  }
		  else{
			flags.N = 0;
		  }
		  if(!rf[alu.instr.lsli.rd]){
			flags.Z = 1;
		  }
		  else {
			flags.Z = 0;
		  }
		  
          break;
        case ALU_LSRI:
		  setCarryOverflow(rf[alu.instr.lsli.rd], rf[alu.instr.lsli.rm],OF_SHIFT);
          rf.write(alu.instr.lsli.rd, rf[alu.instr.lsli.rm] >> alu.instr.lsli.imm);
          if(rf[alu.instr.lsli.rd] & 1 << 31){
			flags.N = 1;
		  }
		  else{
			flags.N = 0;
		  }
		  if(!rf[alu.instr.lsli.rd]){
			flags.Z = 1;
		  }
		  else {
			flags.Z = 0;
		  }
		  break;
        case ALU_ASRI:
		  setCarryOverflow(rf[alu.instr.lsli.rd], rf[alu.instr.lsli.rm],OF_SHIFT);
          rf.write(alu.instr.lsli.rd, rf[alu.instr.lsli.rm] >> alu.instr.lsli.imm);
		  if(rf[alu.instr.lsli.rd] & 1 << 31){
			flags.N = 1;
		  }
		  else{
			flags.N = 0;
		  }
		  if(!rf[alu.instr.lsli.rd]){
			flags.Z = 1;
		  }
		  else {
			flags.Z = 0;
		  }
          break;
        case ALU_ADDR:
		  setCarryOverflow(rf[alu.instr.addr.rn], rf[alu.instr.addr.rm],OF_ADD);
          rf.write(alu.instr.addr.rd, rf[alu.instr.addr.rn] + rf[alu.instr.addr.rm]);
		  if(rf[alu.instr.addr.rd] & 1 << 31){
			flags.N = 1;
		  }
		  else{
			flags.N = 0;
		  }
		  if(!rf[alu.instr.addr.rd]){
			flags.Z = 1;
		  }
		  else {
			flags.Z = 0;
		  }
          break;
        case ALU_SUBR:
		  setCarryOverflow(rf[alu.instr.subr.rn], rf[alu.instr.subr.rm],OF_SUB);
          rf.write(alu.instr.subr.rd, rf[alu.instr.subr.rn] - rf[alu.instr.subr.rm]);
		  if(rf[alu.instr.subr.rd] & 1 << 31){
			flags.N = 1;
		  }
		  else{
			flags.N = 0;
		  }
		  if(!rf[alu.instr.subr.rd]){
			flags.Z = 1;
		  }
		  else {
			flags.Z = 0;
		  }
          break;
        case ALU_ADD3I:
		  setCarryOverflow(rf[alu.instr.add3i.rn], alu.instr.add3i.imm, OF_ADD);
          rf.write(alu.instr.add3i.rd, rf[alu.instr.add3i.rn] + alu.instr.add3i.imm);
		  if(rf[alu.instr.add3i.rd] & 1 << 31){
			flags.N = 1;
		  }
		  else{
			flags.N = 0;
		  }
		  if(!rf[alu.instr.add3i.rd]){
			flags.Z = 1;
		  }
		  else {
			flags.Z = 0;
		  }
          break;
        case ALU_SUB3I:
		  setCarryOverflow(rf[alu.instr.sub3i.rn], alu.instr.sub3i.imm ,OF_SUB);
          rf.write(alu.instr.sub3i.rd, rf[alu.instr.sub3i.rn] - alu.instr.sub3i.imm);
		  if(rf[alu.instr.sub3i.rd] & 1 << 31){
			flags.N = 1;
		  }
		  else{
			flags.N = 0;
		  }
		  if(!rf[alu.instr.sub3i.rd]){
			flags.Z = 1;
		  }
		  else {
			flags.Z = 0;
		  }
          break;
        case ALU_MOV:
          rf.write(alu.instr.mov.rdn, alu.instr.mov.imm);
		  if(rf[alu.instr.mov.rdn] & 1 << 31){
			flags.N = 1;
		  }
		  else{
			flags.N = 0;
		  }
		  if(!rf[alu.instr.mov.rdn]){
			flags.Z = 1;
		  }
		  else {
			flags.Z = 0;
		  }
          break;
        case ALU_CMP:
		  setCarryOverflow(rf[alu.instr.cmp.rdn], alu.instr.sub3i.imm, OF_SUB);
		  signed int signedResult;
		  
		  signedResult = rf[alu.instr.cmp.rdn] + (~alu.instr.cmp.imm + 1);
		  if(signedResult & 1 << 31){
			flags.N = 1;
		  }
		  else{
			flags.N = 0;
		  }
		  if(signedResult == 0){
			flags.Z = 1;
		  }
		  else {
			flags.Z = 0;
		  }
          break;
        case ALU_ADD8I:
		  setCarryOverflow(rf[alu.instr.add8i.rdn], alu.instr.add8i.imm, OF_ADD);
          rf.write(alu.instr.add8i.rdn, rf[alu.instr.add8i.rdn] + alu.instr.add8i.imm);
		  if(rf[alu.instr.add8i.rdn] & 1 << 31){
			flags.N = 1;
		  }
		  else{
			flags.N = 0;
		  }
		  if(!rf[alu.instr.add8i.rdn]){
			flags.Z = 1;
		  }
		  else {
			flags.Z = 0;
		  }
          break;
        case ALU_SUB8I:
		  setCarryOverflow(rf[alu.instr.sub8i.rdn], alu.instr.sub8i.imm ,OF_SUB);
          rf.write(alu.instr.sub8i.rdn, rf[alu.instr.sub8i.rdn] - alu.instr.sub8i.imm);
		  if(rf[alu.instr.sub8i.rdn] & 1 << 31){
			flags.N = 1;
		  }
		  else{
			flags.N = 0;
		  }
		  if(!rf[alu.instr.sub8i.rdn]){
			flags.Z = 1;
		  }
		  else {
			flags.Z = 0;
		  }
          break;
        default:
			cout << "[ERROR] Unknown ALU Instruction to be executed" << endl;
          break;
      }
      break;
    case DP:
      dp_ops = decode(dp);
	  switch(dp_ops){
	    case DP_AND_OP:
		  rf.write(rf[dp.instr.DP_Instr.rdn], rf[dp.instr.DP_Instr.rdn] & rf[dp.instr.DP_Instr.rm]); 
		  if(rf[dp.instr.DP_Instr.rdn] & 1 << 31){
			flags.N = 1;
		  }
		  else{
			flags.N = 0;
		  }
		  if(!rf[dp.instr.DP_Instr.rdn]){
			flags.Z = 1;
		  }
		  else {
			flags.Z = 0;
		  }
		  flags.C = 0;
		  break;
		case DP_EOR_OP:
		  rf.write(rf[dp.instr.DP_Instr.rdn], rf[dp.instr.DP_Instr.rdn] ^ rf[dp.instr.DP_Instr.rm]); 
		  if(rf[dp.instr.DP_Instr.rdn] & 1 << 31){
			flags.N = 1;
		  }
		  else{
			flags.N = 0;
		  }
		  if(!rf[dp.instr.DP_Instr.rdn]){
			flags.Z = 1;
		  }
		  else {
			flags.Z = 0;
		  }
		  flags.C = 0;
		  break;
		case DP_LSL_OP:
		  setCarryOverflow(rf[dp.instr.DP_Instr.rdn], rf[dp.instr.DP_Instr.rm], OF_SHIFT);
		  rf.write(rf[dp.instr.DP_Instr.rdn], rf[dp.instr.DP_Instr.rdn] << rf[dp.instr.DP_Instr.rm]); 
		  if(rf[dp.instr.DP_Instr.rdn] & 1 << 31){
			flags.N = 1;
		  }
		  else{
			flags.N = 0;
		  }
		  if(!rf[dp.instr.DP_Instr.rdn]){
			flags.Z = 1;
		  }
		  else {
			flags.Z = 0;
		  }
		  break;
		case DP_LSR_OP:
		  setCarryOverflow(rf[dp.instr.DP_Instr.rdn], rf[dp.instr.DP_Instr.rm], OF_SHIFT);
		  rf.write(rf[dp.instr.DP_Instr.rdn], (unsigned int)(rf[dp.instr.DP_Instr.rdn]) >> rf[dp.instr.DP_Instr.rm]); 
		  if(rf[dp.instr.DP_Instr.rdn] & 1 << 31){
			flags.N = 1;
		  }
		  else{
			flags.N = 0;
		  }
		  if(!rf[dp.instr.DP_Instr.rdn]){
			flags.Z = 1;
		  }
		  else {
			flags.Z = 0;
		  }
		  break;
		case DP_ASR_OP:
		  setCarryOverflow(rf[dp.instr.DP_Instr.rdn], rf[dp.instr.DP_Instr.rm], OF_SHIFT);
		  rf.write(rf[dp.instr.DP_Instr.rdn], rf[dp.instr.DP_Instr.rdn] >> rf[dp.instr.DP_Instr.rm]); 
		  if(rf[dp.instr.DP_Instr.rdn] & 1 << 31){
			flags.N = 1;
		  }
		  else{
			flags.N = 0;
		  }
		  if(!rf[dp.instr.DP_Instr.rdn]){
			flags.Z = 1;
		  }
		  else {
			flags.Z = 0;
		  }
		  break;
		case DP_ADC_OP:
		  setCarryOverflow(rf[dp.instr.DP_Instr.rdn], rf[dp.instr.DP_Instr.rm], OF_ADD);
		  rf.write(rf[dp.instr.DP_Instr.rdn], rf[dp.instr.DP_Instr.rdn] + rf[dp.instr.DP_Instr.rm] + flags.C); 
		  if(rf[dp.instr.DP_Instr.rdn] & 1 << 31){
			flags.N = 1;
		  }
		  else{
			flags.N = 0;
		  }
		  if(!rf[dp.instr.DP_Instr.rdn]){
			flags.Z = 1;
		  }
		  else {
			flags.Z = 0;
		  }
		  break;
		case DP_SBC_OP:
		  setCarryOverflow(rf[dp.instr.DP_Instr.rdn], rf[dp.instr.DP_Instr.rm], OF_SUB);
		  rf.write(rf[dp.instr.DP_Instr.rdn], rf[dp.instr.DP_Instr.rdn] - rf[dp.instr.DP_Instr.rm] - flags.C); 
		  if(rf[dp.instr.DP_Instr.rdn] & 1 << 31){
			flags.N = 1;
		  }
		  else{
			flags.N = 0;
		  }
		  if(!rf[dp.instr.DP_Instr.rdn]){
			flags.Z = 1;
		  }
		  else {
			flags.Z = 0;
		  }
		  break;
		case DP_ROR_OP:
		  //not required
		  break;
		case DP_TST_OP:
		  if(rf[dp.instr.DP_Instr.rdn] & 1 << 31){
			flags.N = 1;
		  }
		  else{
			flags.N = 0;
		  }
		  if(!rf[dp.instr.DP_Instr.rdn]){
			flags.Z = 1;
		  }
		  else {
			flags.Z = 0;
		  }
		  break;
		case DP_RSB_OP:
		{
		  //not required
		  break;
		}
		case DP_CMP_OP:
		  signed int signedResult;
		  setCarryOverflow(rf[dp.instr.DP_Instr.rdn], rf[dp.instr.DP_Instr.rm], OF_SUB);
		  signedResult = rf[dp.instr.DP_Instr.rdn] + (~rf[dp.instr.DP_Instr.rm] + 1);
		  if(signedResult & 1 << 31){
			flags.N = 1;
		  }
		  else{
			flags.N = 0;
		  }
		  if(signedResult == 0){
			flags.Z = 1;
		  }
		  else {
			flags.Z = 0;
		  }
		  break;
		case DP_CMN_OP:
		 //not required
		  break;
		case DP_MUL_OP:
		 //not required
		  break;
		case DP_BIC_OP:
		 //not required
		  break;
		case DP_MVN_OP:
		 //not required
		  break;
	  }
      break;
    case SPECIAL:
      sp_ops = decode(sp);
      switch(sp_ops) {
        case SP_MOV:
          if (sp.instr.mov.d == 1 && sp.instr.mov.rd == 5) {
            rf.write(SP_REG, rf[sp.instr.mov.rm]);
          }
          else {
		    if(sp.instr.mov.d == 1){
				rf.write(sp.instr.mov.rd + 8, rf[sp.instr.mov.rm]);
			}
			else{
				rf.write(sp.instr.mov.rd, rf[sp.instr.mov.rm]);
			}
          }
          break;
		case SP_ADD:
		 if (sp.instr.add.d == 1 && sp.instr.add.rd == 5 || sp.instr.add.rm == 13) {
		    rf.write(SP_REG, SP + rf[sp.instr.add.rm]);
         }
         else {
            rf.write(sp.instr.add.rd, rf[sp.instr.add.rd] + rf[sp.instr.add.rm]);
         }
      }
      break;
    case LD_ST:
      // You'll want to use these load and store models
      // to implement ldrb/strb, ldm/stm and push/pop
      ldst_ops = decode(ld_st);
      switch(ldst_ops) {
        case STRI:
          addr = rf[ld_st.instr.ld_st_imm.rn] + ld_st.instr.ld_st_imm.imm * 4;
          dmem.write(addr, rf[ld_st.instr.ld_st_imm.rt]);
          break;
        case LDRI:
          addr = rf[ld_st.instr.ld_st_imm.rn] + ld_st.instr.ld_st_imm.imm * 4;
          rf.write(ld_st.instr.ld_st_imm.rt, dmem[addr]);
          break;
		case STRSP:
		  addr = SP + ld_st.instr.ld_st_sp.imm * 4;
		  dmem.write(addr, rf[ld_st.instr.ld_st_sp.rt]);
		  break;
		case LDRSP:
		  addr = SP + ld_st.instr.ld_st_sp.imm * 4;
		  rf.write(ld_st.instr.ld_st_sp.rt, dmem[addr]);
		  break;
      }
      break;
    case MISC:
      misc_ops = decode(misc);
      switch(misc_ops) {
        case MISC_PUSH:
			if (misc.instr.push.reg_list & 1) {
				dmem.write(SP, rf[0]);
				rf.write(SP_REG, SP - 4);
			}
			if (misc.instr.push.reg_list & 2) {
				dmem.write(SP, rf[1]);
				rf.write(SP_REG, SP - 4);
			}
			if (misc.instr.push.reg_list & 4) {
				dmem.write(SP, rf[2]);
				rf.write(SP_REG, SP - 4);
			}
			if (misc.instr.push.reg_list & 8) {
				dmem.write(SP, rf[3]);
				rf.write(SP_REG, SP - 4);
			}
			if (misc.instr.push.reg_list & 16) {
				dmem.write(SP, rf[4]);
				rf.write(SP_REG, SP - 4);
			}
			if (misc.instr.push.reg_list & 32) {
				dmem.write(SP, rf[5]);
				rf.write(SP_REG, SP - 4);
			} 
			if (misc.instr.push.reg_list & 64) {
				dmem.write(SP, rf[6]);
				rf.write(SP_REG, SP - 4);
			}
			if (misc.instr.push.reg_list & 128) {
				dmem.write(SP, rf[7]);
				rf.write(SP_REG, SP - 4);
			}
			if (misc.instr.push.m) {
				dmem.write(SP, LR);
				rf.write(SP_REG, SP - 4);
			}
          break;
        case MISC_POP:
		    if (misc.instr.pop.m) {
				rf.write(PC_REG, dmem[SP]);
				rf.write(SP_REG, SP + 4);
		    }
		    if (misc.instr.pop.reg_list & 128) {
				rf.write(7, dmem[SP]);
				rf.write(SP_REG, SP + 4);
		    }
		    if (misc.instr.pop.reg_list & 64) {
				rf.write(6, dmem[SP]);
				rf.write(SP_REG, SP + 4);
		    }
		    if (misc.instr.pop.reg_list & 32) {
				rf.write(5, dmem[SP]);
				rf.write(SP_REG, SP + 4);
		    }
		    if (misc.instr.pop.reg_list & 16) {
				rf.write(4, dmem[SP]);
				rf.write(SP_REG, SP + 4);
		    }
		    if (misc.instr.pop.reg_list & 8) {
				rf.write(3, dmem[SP]);
				rf.write(SP_REG, SP + 4);
		    }
		    if (misc.instr.pop.reg_list & 4) {
				rf.write(2, dmem[SP]);
				rf.write(SP_REG, SP + 4);
		    }
		    if (misc.instr.pop.reg_list & 2) {
				rf.write(1, dmem[SP]);
				rf.write(SP_REG, SP + 4);
		    }
			if (misc.instr.pop.reg_list & 1) {
				rf.write(0, dmem[SP]);
				rf.write(SP_REG, SP + 4);
		    }
          break;
        case MISC_SUB:
          rf.write(SP_REG, SP - (misc.instr.sub.imm*4));
          break;
        case MISC_ADD:
          rf.write(SP_REG, SP + (misc.instr.add.imm*4));
          break;
      }
      break;
    case COND:
      decode(cond);
      // Once you've completed the checkCondition function,
      // this should work for all your conditional branches.
      if (checkCondition(cond.instr.b.cond)){
        rf.write(PC_REG, PC + 2 * signExtend8to32ui(cond.instr.b.imm) + 2);
      }
      break;
    case UNCOND:
      decode(uncond);
      rf.write(PC_REG, PC + 2 * signExtend8to32ui(cond.instr.b.imm) + 2);
      break;
    case LDM:
      decode(ldm);
		if (ldm.instr.ldm.reg_list & 128) {
			tempMov -= 4;
			rf.write(7, dmem[ldm.instr.ldm.rn] + tempMov);
		}
		if (ldm.instr.ldm.reg_list & 64) {
			tempMov -= 4;
			rf.write(6, dmem[ldm.instr.ldm.rn] + tempMov);
		}
		if (ldm.instr.ldm.reg_list & 32) {
			tempMov -= 4;
			rf.write(5, dmem[ldm.instr.ldm.rn] + tempMov);
		} 
		if (ldm.instr.ldm.reg_list & 16) {
			tempMov -= 4;
			rf.write(4, dmem[ldm.instr.ldm.rn] + tempMov);
		}
		if (ldm.instr.ldm.reg_list & 8) {
			tempMov -= 4;
			rf.write(3, dmem[ldm.instr.ldm.rn] + tempMov);
		}
		if (ldm.instr.ldm.reg_list & 4) {
			tempMov -= 4;
			rf.write(2, dmem[ldm.instr.ldm.rn] + tempMov);
		}
		if (ldm.instr.ldm.reg_list & 2) {
			tempMov -= 4;
			rf.write(1, dmem[ldm.instr.ldm.rn] + tempMov);
		}
		if (ldm.instr.ldm.reg_list & 1) {
			tempMov -= 4;
			rf.write(0, dmem[ldm.instr.ldm.rn] + tempMov);
		}
		rf.write(ldm.instr.ldm.rn, rf[ldm.instr.ldm.rn] + tempMov);
      break;
    case STM:
      decode(stm);
		if (stm.instr.stm.reg_list & 1) {
			dmem.write(stm.instr.stm.rn, rf[0]);
			tempMov += 4;
		}
		if (stm.instr.stm.reg_list & 2) {
			dmem.write(stm.instr.stm.rn + tempMov, rf[1]);
			tempMov += 4;
		}
		if (stm.instr.stm.reg_list & 4) {
			dmem.write(stm.instr.stm.rn + tempMov, rf[2]);
			tempMov += 4;
		}
		if (stm.instr.stm.reg_list & 8) {
			dmem.write(stm.instr.stm.rn + tempMov, rf[3]);
			tempMov += 4;
		}
		if (stm.instr.stm.reg_list & 16) {
			dmem.write(stm.instr.stm.rn + tempMov, rf[4]);
			tempMov += 4;
		}
		if (stm.instr.stm.reg_list & 32) {
			dmem.write(stm.instr.stm.rn + tempMov, rf[5]);
			tempMov += 4;
		} 
		if (stm.instr.stm.reg_list & 64) {
			dmem.write(stm.instr.stm.rn + tempMov, rf[6]);
			tempMov += 4;
		}
		if (stm.instr.stm.reg_list & 128) {
			dmem.write(stm.instr.stm.rn + tempMov, rf[7]);
			tempMov += 4;
		}
		rf.write(stm.instr.stm.rn, rf[stm.instr.stm.rn] + tempMov);
      break;
    case LDRL:
      decode(ldrl);
	  rf.write(ldrl.instr.ldrl.rt, PC + (ldrl.instr.ldrl.imm*4));
      break;
    case ADD_SP:
      decode(addsp);
      rf.write(addsp.instr.add.rd, SP + (addsp.instr.add.imm*4));
      break;
	case BL:
	  bl_ops = decode(blupper);
      if (bl_ops == BL_UPPER) {
        // PC has already been incremented above
        Data16 instr2 = imem[PC];
        BL_Type bllower(instr2);
        if (blupper.instr.bl_upper.s) {
          addr = static_cast<int>(0xff<<24) | 
                 ((~(bllower.instr.bl_lower.j1 ^ blupper.instr.bl_upper.s))<<23) | 
                 ((~(bllower.instr.bl_lower.j2 ^ blupper.instr.bl_upper.s))<<22) | 
                 ((blupper.instr.bl_upper.imm10)<<12) | 
                 ((bllower.instr.bl_lower.imm11)<<1);
        }
        else { 
          addr = ((blupper.instr.bl_upper.imm10)<<12) | 
                 ((bllower.instr.bl_lower.imm11)<<1); 
        }
        // return address is 4-bytes away from the start of the BL insn 
        rf.write(LR_REG, PC + 2); 
        // Target address is also computed from that point 
        rf.write(PC_REG, PC + 2 + addr); 
      }
      else {
        cerr << "Bad BL format." << endl; exit(1); 
      }
      break; 
    default:
      cout << "[ERROR] Unknown Instruction to be executed" << endl;
      exit(1);
      break;
  }
}  
