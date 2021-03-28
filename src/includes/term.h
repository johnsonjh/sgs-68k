/*	Copyright (c) 1984 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

	

#ifndef TERM_H
#define TERM_H

/*
 * term.h - this file is automatically made from caps and maketerm.ed.
 * Don't make changes directly to term.h.
 *
 * Guard against multiple includes.
 */

#ifndef auto_left_margin
#ident	"@(#)term.h	25.1	12/2/91 Copyright (c) 1991 by Arix Corp."

#define auto_left_margin 		CUR _b1
#define auto_right_margin 		CUR _b2
#define no_esc_ctlc 			CUR _b3
#define ceol_standout_glitch 		CUR _b4
#define eat_newline_glitch 		CUR _b5
#define erase_overstrike 		CUR _b6
#define generic_type 			CUR _b7
#define hard_copy 			CUR _b8
#define has_meta_key 			CUR _b9
#define has_status_line 		CUR _b10
#define insert_null_glitch 		CUR _b11
#define memory_above 			CUR _b12
#define memory_below 			CUR _b13
#define move_insert_mode 		CUR _b14
#define move_standout_mode 		CUR _b15
#define over_strike 			CUR _b16
#define status_line_esc_ok 		CUR _b17
#define dest_tabs_magic_smso 		CUR _b18
#define tilde_glitch 			CUR _b19
#define transparent_underline 		CUR _b20
#define xon_xoff 			CUR _b21
#define needs_xon_xoff 			CURB _needs_xon_xoff
#define prtr_silent 			CURB _prtr_silent
#define hard_cursor 			CURB _hard_cursor
#define non_rev_rmcup 			CURB _non_rev_rmcup
#define no_pad_char 			CURB _no_pad_char
#define non_dest_scroll_region 		CURB _non_dest_scroll_region
#define columns 			CUR _c1
#define init_tabs 			CUR _c2
#define lines 				CUR _c3
#define lines_of_memory 		CUR _c4
#define magic_cookie_glitch 		CUR _c5
#define padding_baud_rate 		CUR _c6
#define virtual_terminal 		CUR _c7
#define width_status_line 		CUR _c8
#define num_labels 			CURN _num_labels
#define label_height 			CURN _labl_height
#define label_width 			CURN _labl_width
#define max_attributes 			CURN _max_attributes
#define maximum_windows 		CURN _maximum_windows
#define back_tab 			CUR _Vr2_Astrs._s1
#define bell 				CUR _Vr2_Astrs._s2
#define carriage_return 		CUR _Vr2_Astrs._s3
#define change_scroll_region 		CUR _Vr2_Astrs._s4
#define clear_all_tabs 			CUR _Vr2_Astrs._s5
#define clear_screen 			CUR _Vr2_Astrs._s6
#define clr_eol 			CUR _Vr2_Astrs._s7
#define clr_eos 			CUR _Vr2_Astrs._s8
#define column_address 			CUR _Vr2_Astrs._s9
#define command_character 		CUR _Vr2_Astrs._s10
#define cursor_address 			CUR _Vr2_Astrs._s11
#define cursor_down 			CUR _Vr2_Astrs._s12
#define cursor_home 			CUR _Vr2_Astrs._s13
#define cursor_invisible 		CUR _Vr2_Astrs._s14
#define cursor_left 			CUR _Vr2_Astrs._s15
#define cursor_mem_address 		CUR _Vr2_Astrs._s16
#define cursor_normal 			CUR _Vr2_Astrs._s17
#define cursor_right 			CUR _Vr2_Astrs._s18
#define cursor_to_ll 			CUR _Vr2_Astrs._s19
#define cursor_up 			CUR _Vr2_Astrs._s20
#define cursor_visible 			CUR _Vr2_Astrs._s21
#define delete_character 		CUR _Vr2_Astrs._s22
#define delete_line 			CUR _Vr2_Astrs._s23
#define dis_status_line 		CUR _Vr2_Astrs._s24
#define down_half_line 			CUR _Vr2_Astrs._s25
#define enter_alt_charset_mode 		CUR _Vr2_Astrs._s26
#define enter_blink_mode 		CUR _Vr2_Astrs._s27
#define enter_bold_mode 		CUR _Vr2_Astrs._s28
#define enter_ca_mode 			CUR _Vr2_Astrs._s29
#define enter_delete_mode 		CUR _Vr2_Astrs._s30
#define enter_dim_mode 			CUR _Vr2_Astrs._s31
#define enter_insert_mode 		CUR _Vr2_Astrs._s32
#define enter_secure_mode 		CUR _Vr2_Astrs._s33
#define enter_protected_mode 		CUR _Vr2_Astrs._s34
#define enter_reverse_mode 		CUR _Vr2_Astrs._s35
#define enter_standout_mode 		CUR _Vr2_Astrs._s36
#define enter_underline_mode 		CUR _Vr2_Astrs._s37
#define erase_chars 			CUR _Vr2_Astrs._s38
#define exit_alt_charset_mode 		CUR _Vr2_Astrs._s39
#define exit_attribute_mode 		CUR _Vr2_Astrs._s40
#define exit_ca_mode 			CUR _Vr2_Astrs._s41
#define exit_delete_mode 		CUR _Vr2_Astrs._s42
#define exit_insert_mode 		CUR _Vr2_Astrs._s43
#define exit_standout_mode 		CUR _Vr2_Astrs._s44
#define exit_underline_mode 		CUR _Vr2_Astrs._s45
#define flash_screen 			CUR _Vr2_Astrs._s46
#define form_feed 			CUR _Vr2_Astrs._s47
#define from_status_line 		CUR _Vr2_Astrs._s48
#define init_1string 			CUR _Vr2_Astrs._s49
#define init_2string 			CUR _Vr2_Astrs._s50
#define init_3string 			CUR _Vr2_Astrs._s51
#define init_file 			CUR _Vr2_Astrs._s52
#define insert_character 		CUR _Vr2_Astrs._s53
#define insert_line 			CUR _Vr2_Astrs._s54
#define insert_padding 			CUR _Vr2_Astrs._s55
#define key_backspace 			CUR _Vr2_Astrs._s56
#define key_catab 			CUR _Vr2_Astrs._s57
#define key_clear 			CUR _Vr2_Astrs._s58
#define key_ctab 			CUR _Vr2_Astrs._s59
#define key_dc 				CUR _Vr2_Astrs._s60
#define key_dl 				CUR _Vr2_Astrs._s61
#define key_down 			CUR _Vr2_Astrs._s62
#define key_eic 			CUR _Vr2_Astrs._s63
#define key_eol 			CUR _Vr2_Astrs._s64
#define key_eos 			CUR _Vr2_Astrs._s65
#define key_f0 				CUR _Vr2_Astrs._s66
#define key_f1 				CUR _Vr2_Astrs._s67
#define key_f10 			CUR _Vr2_Astrs._s68
#define key_f2 				CUR _Vr2_Astrs._s69
#define key_f3 				CUR _Vr2_Astrs._s70
#define key_f4 				CUR _Vr2_Astrs._s71
#define key_f5 				CUR _Vr2_Astrs._s72
#define key_f6 				CUR _Vr2_Astrs._s73
#define key_f7 				CUR _Vr2_Astrs._s74
#define key_f8 				CUR _Vr2_Astrs._s75
#define key_f9 				CUR _Vr2_Astrs._s76
#define key_home 			CUR _Vr2_Astrs._s77
#define key_ic 				CUR _Vr2_Astrs._s78
#define key_il 				CUR _Vr2_Astrs._s79
#define key_left 			CUR _Vr2_Astrs._s80
#define key_ll 				CUR _Vr2_Astrs._s81
#define key_npage 			CUR _Vr2_Astrs._s82
#define key_ppage 			CUR _Vr2_Astrs._s83
#define key_right 			CUR _Vr2_Astrs._s84
#define key_sf 				CUR _Vr2_Astrs._s85
#define key_sr 				CUR _Vr2_Astrs._s86
#define key_stab 			CUR _Vr2_Astrs._s87
#define key_up 				CUR _Vr2_Astrs._s88
#define keypad_local 			CUR _Vr2_Astrs._s89
#define keypad_xmit 			CUR _Vr2_Astrs._s90
#define lab_f0 				CUR _Vr2_Astrs._s91
#define lab_f1 				CUR _Vr2_Astrs._s92
#define lab_f10 			CUR _Vr2_Astrs._s93
#define lab_f2 				CUR _Vr2_Astrs._s94
#define lab_f3 				CUR _Vr2_Astrs._s95
#define lab_f4 				CUR _Vr2_Astrs._s96
#define lab_f5 				CUR _Vr2_Astrs._s97
#define lab_f6 				CUR _Vr2_Astrs._s98
#define lab_f7 				CUR _Vr2_Astrs._s99
#define lab_f8 				CUR _Vr2_Astrs._s100
#define lab_f9 				CUR _Vr2_Bstrs._s101
#define meta_off 			CUR _Vr2_Bstrs._s102
#define meta_on 			CUR _Vr2_Bstrs._s103
#define newline 			CUR _Vr2_Bstrs._s104
#define pad_char 			CUR _Vr2_Bstrs._s105
#define parm_dch 			CUR _Vr2_Bstrs._s106
#define parm_delete_line 		CUR _Vr2_Bstrs._s107
#define parm_down_cursor 		CUR _Vr2_Bstrs._s108
#define parm_ich 			CUR _Vr2_Bstrs._s109
#define parm_index 			CUR _Vr2_Bstrs._s110
#define parm_insert_line 		CUR _Vr2_Bstrs._s111
#define parm_left_cursor 		CUR _Vr2_Bstrs._s112
#define parm_right_cursor 		CUR _Vr2_Bstrs._s113
#define parm_rindex 			CUR _Vr2_Bstrs._s114
#define parm_up_cursor 			CUR _Vr2_Bstrs._s115
#define pkey_key 			CUR _Vr2_Bstrs._s116
#define pkey_local 			CUR _Vr2_Bstrs._s117
#define pkey_xmit 			CUR _Vr2_Bstrs._s118
#define print_screen 			CUR _Vr2_Bstrs._s119
#define prtr_off 			CUR _Vr2_Bstrs._s120
#define prtr_on 			CUR _Vr2_Bstrs._s121
#define repeat_char 			CUR _Vr2_Bstrs._s122
#define reset_1string 			CUR _Vr2_Bstrs._s123
#define reset_2string 			CUR _Vr2_Bstrs._s124
#define reset_3string 			CUR _Vr2_Bstrs._s125
#define reset_file 			CUR _Vr2_Bstrs._s126
#define restore_cursor 			CUR _Vr2_Bstrs._s127
#define row_address 			CUR _Vr2_Bstrs._s128
#define save_cursor 			CUR _Vr2_Bstrs._s129
#define scroll_forward 			CUR _Vr2_Bstrs._s130
#define scroll_reverse 			CUR _Vr2_Bstrs._s131
#define set_attributes 			CUR _Vr2_Bstrs._s132
#define set_tab 			CUR _Vr2_Bstrs._s133
#define set_window 			CUR _Vr2_Bstrs._s134
#define tab 				CUR _Vr2_Bstrs._s135
#define to_status_line 			CUR _Vr2_Bstrs._s136
#define underline_char 			CUR _Vr2_Bstrs._s137
#define up_half_line 			CUR _Vr2_Bstrs._s138
#define init_prog 			CUR _Vr2_Bstrs._s139
#define key_a1 				CUR _Vr2_Bstrs._s140
#define key_a3 				CUR _Vr2_Bstrs._s141
#define key_b2 				CUR _Vr2_Bstrs._s142
#define key_c1 				CUR _Vr2_Bstrs._s143
#define key_c3 				CUR _Vr2_Bstrs._s144
#define prtr_non 			CUR _Vr2_Bstrs._s145
#define char_padding 			CURS strs2._char_padding
#define acs_chars 			CURS strs2._acs_chars
#define plab_norm 			CURS strs2._plab_norm
#define key_btab 			CURS strs2._ky_btab
#define enter_xon_mode 			CURS strs2._entr_xon_mode
#define exit_xon_mode 			CURS strs2._exit_xon_mode
#define enter_am_mode 			CURS strs2._entr_am_mode
#define exit_am_mode 			CURS strs2._exit_am_mode
#define xon_character 			CURS strs2._xon_character
#define xoff_character 			CURS strs2._xoff_character
#define ena_acs 			CURS strs2._ena_acs
#define label_on 			CURS strs2._labl_on
#define label_off 			CURS strs2._labl_off
#define key_beg 			CURS strs2._ky_beg
#define key_cancel 			CURS strs2._ky_cancel
#define key_close 			CURS strs2._ky_close
#define key_command 			CURS strs2._ky_command
#define key_copy 			CURS strs2._ky_copy
#define key_create 			CURS strs2._ky_create
#define key_end 			CURS strs2._ky_end
#define key_enter 			CURS strs2._ky_enter
#define key_exit 			CURS strs2._ky_exit
#define key_find 			CURS strs2._ky_find
#define key_help 			CURS strs2._ky_help
#define key_mark 			CURS strs2._ky_mark
#define key_message 			CURS strs2._ky_message
#define key_move 			CURS strs2._ky_move
#define key_next 			CURS strs2._ky_next
#define key_open 			CURS strs2._ky_open
#define key_options 			CURS strs2._ky_options
#define key_previous 			CURS strs2._ky_previous
#define key_print 			CURS strs2._ky_print
#define key_redo 			CURS strs2._ky_redo
#define key_reference 			CURS strs2._ky_reference
#define key_refresh 			CURS strs2._ky_refresh
#define key_replace 			CURS strs2._ky_replace
#define key_restart 			CURS strs2._ky_restart
#define key_resume 			CURS strs2._ky_resume
#define key_save 			CURS strs2._ky_save
#define key_suspend 			CURS strs2._ky_suspend
#define key_undo 			CURS strs2._ky_undo
#define key_sbeg 			CURS strs2._ky_sbeg
#define key_scancel 			CURS strs2._ky_scancel
#define key_scommand 			CURS strs2._ky_scommand
#define key_scopy 			CURS strs2._ky_scopy
#define key_screate 			CURS strs2._ky_screate
#define key_sdc 			CURS strs2._ky_sdc
#define key_sdl 			CURS strs2._ky_sdl
#define key_select 			CURS strs2._ky_select
#define key_send 			CURS strs2._ky_send
#define key_seol 			CURS strs2._ky_seol
#define key_sexit 			CURS strs2._ky_sexit
#define key_sfind 			CURS strs2._ky_sfind
#define key_shelp 			CURS strs2._ky_shelp
#define key_shome 			CURS strs2._ky_shome
#define key_sic 			CURS strs3._ky_sic
#define key_sleft 			CURS strs3._ky_sleft
#define key_smessage 			CURS strs3._ky_smessage
#define key_smove 			CURS strs3._ky_smove
#define key_snext 			CURS strs3._ky_snext
#define key_soptions 			CURS strs3._ky_soptions
#define key_sprevious 			CURS strs3._ky_sprevious
#define key_sprint 			CURS strs3._ky_sprint
#define key_sredo 			CURS strs3._ky_sredo
#define key_sreplace 			CURS strs3._ky_sreplace
#define key_sright 			CURS strs3._ky_sright
#define key_srsume 			CURS strs3._ky_srsume
#define key_ssave 			CURS strs3._ky_ssave
#define key_ssuspend 			CURS strs3._ky_ssuspend
#define key_sundo 			CURS strs3._ky_sundo
#define req_for_input 			CURS strs3._req_for_input
#define key_f11 			CURS strs3._ky_f11
#define key_f12 			CURS strs3._ky_f12
#define key_f13 			CURS strs3._ky_f13
#define key_f14 			CURS strs3._ky_f14
#define key_f15 			CURS strs3._ky_f15
#define key_f16 			CURS strs3._ky_f16
#define key_f17 			CURS strs3._ky_f17
#define key_f18 			CURS strs3._ky_f18
#define key_f19 			CURS strs3._ky_f19
#define key_f20 			CURS strs3._ky_f20
#define key_f21 			CURS strs3._ky_f21
#define key_f22 			CURS strs3._ky_f22
#define key_f23 			CURS strs3._ky_f23
#define key_f24 			CURS strs3._ky_f24
#define key_f25 			CURS strs3._ky_f25
#define key_f26 			CURS strs3._ky_f26
#define key_f27 			CURS strs3._ky_f27
#define key_f28 			CURS strs3._ky_f28
#define key_f29 			CURS strs3._ky_f29
#define key_f30 			CURS strs3._ky_f30
#define key_f31 			CURS strs3._ky_f31
#define key_f32 			CURS strs3._ky_f32
#define key_f33 			CURS strs3._ky_f33
#define key_f34 			CURS strs3._ky_f34
#define key_f35 			CURS strs3._ky_f35
#define key_f36 			CURS strs3._ky_f36
#define key_f37 			CURS strs3._ky_f37
#define key_f38 			CURS strs3._ky_f38
#define key_f39 			CURS strs3._ky_f39
#define key_f40 			CURS strs3._ky_f40
#define key_f41 			CURS strs3._ky_f41
#define key_f42 			CURS strs3._ky_f42
#define key_f43 			CURS strs3._ky_f43
#define key_f44 			CURS strs3._ky_f44
#define key_f45 			CURS strs3._ky_f45
#define key_f46 			CURS strs3._ky_f46
#define key_f47 			CURS strs3._ky_f47
#define key_f48 			CURS strs3._ky_f48
#define key_f49 			CURS strs3._ky_f49
#define key_f50 			CURS strs3._ky_f50
#define key_f51 			CURS strs3._ky_f51
#define key_f52 			CURS strs3._ky_f52
#define key_f53 			CURS strs3._ky_f53
#define key_f54 			CURS strs3._ky_f54
#define key_f55 			CURS strs3._ky_f55
#define key_f56 			CURS strs3._ky_f56
#define key_f57 			CURS strs3._ky_f57
#define key_f58 			CURS strs3._ky_f58
#define key_f59 			CURS strs3._ky_f59
#define key_f60 			CURS strs3._ky_f60
#define key_f61 			CURS strs3._ky_f61
#define key_f62 			CURS strs3._ky_f62
#define key_f63 			CURS strs3._ky_f63
#define clr_bol 			CURS strs3._clr_bol
#define clear_margins 			CURS strs3._clear_margins
#define set_left_margin 		CURS strs3._set_left_margin
#define set_right_margin 		CURS strs3._set_right_margin
#define label_format 			CURS strs3._labl_format
#define set_clock 			CURS strs3._set_clock
#define display_clock 			CURS strs3._display_clock
#define remove_clock 			CURS strs3._remove_clock
#define create_window 			CURS strs3._create_window
#define goto_window 			CURS strs3._goto_window
#define hangup 				CURS strs3._hangup
#define dial_phone 			CURS strs3._dial_phone
#define quick_dial 			CURS strs3._quick_dial
#define tone 				CURS strs3._tone
#define pulse 				CURS strs3._pulse
#define flash_hook 			CURS strs3._flash_hook
#define fixed_pause 			CURS strs3._fixed_pause
#define wait_tone 			CURS strs3._wait_tone
#define user0 				CURS strs3._user0
#define user1 				CURS strs3._user1
#define user2 				CURS strs3._user2
#define user3 				CURS strs3._user3
#define user4 				CURS strs3._user4
#define user5 				CURS strs3._user5
#define user6 				CURS strs3._user6
#define user7 				CURS strs3._user7
#define user8 				CURS strs3._user8
#define user9 				CURS strs3._user9

typedef char *charptr;
struct strs {
    charptr
	_back_tab,		/* Back tab */
	_bell,			/* Audible signal (bell) */
	_carriage_return,	/* Carriage return (*) */
	_change_scroll_region,	/* change to lines #1 thru #2 (vt100) (G) */
	_clear_all_tabs,	/* Clear all tab stops. */
	_clear_screen,		/* Clear screen (*) */
	_clr_eol,		/* Clear to end of line */
	_clr_eos,		/* Clear to end of display (*) */
	_column_address,	/* Set cursor column (G) */
	_command_character,	/* Term. settable cmd char in prototype */
	_crsr_address,		/* Cursor motion to row #1 col #2 (G) */
	_crsr_down,		/* Down one line */
	_crsr_home,		/* Home cursor (if no cup) */
	_crsr_invisible,	/* Make cursor invisible */
	_crsr_left,		/* Move cursor left one space. */
	_crsr_mem_address,	/* Memory relative cursor addressing. */
	_crsr_normal,		/* Make cursor appear normal (undo vs/vi) */
	_crsr_right,		/* Non-destructive space (cursor right) */
	_crsr_to_ll,		/* Last line, first column (if no cup) */
	_crsr_up,		/* Upline (cursor up) */
	_crsr_visible,		/* Make cursor very visible */
	_dlt_character,	/* Delete character (*) */
	_dlt_line,		/* Delete line (*) */
	_dis_status_line,	/* Disable status line */
	_down_half_line,	/* Half-line down (forward 1/2 linefeed) */
	_entr_alt_charset_mode,	/* Start alternate character set */
	_entr_blink_mode,	/* Turn on blinking */
	_entr_bold_mode,	/* Turn on bold (extra bright) mode */
	_entr_ca_mode,		/* String to begin programs that use cup */
	_entr_delete_mode,	/* Delete mode (enter) */
	_entr_dim_mode,		/* Turn on half-bright mode */
	_entr_insert_mode,	/* Insert mode (enter); */
	_entr_secure_mode,	/* Turn on blank mode (chars invisible) */
	_entr_protected_mode,	/* Turn on protected mode */
	_entr_reverse_mode,	/* Turn on reverse video mode */
	_entr_standout_mode,	/* Begin stand out mode */
	_entr_underline_mode,	/* Start underscore mode */
	_erase_chars,		/* Erase #1 characters (G) */
	_exit_alt_charset_mode,	/* End alternate character set */
	_exit_attribute_mode,	/* Turn off all attributes */
	_exit_ca_mode,		/* String to end programs that use cup */
	_exit_delete_mode,	/* End delete mode */
	_exit_insert_mode,	/* End insert mode; */
	_exit_standout_mode,	/* End stand out mode */
	_exit_underline_mode,	/* End underscore mode */
	_flash_screen,		/* Visible bell (may not move cursor) */
	_form_feed,		/* Hardcopy terminal page eject (*) */
	_from_status_line,	/* Return from status line */
	_init_1string,		/* Terminal initialization string */
	_init_2string,		/* Terminal initialization string */
	_init_3string,		/* Terminal initialization string */
	_init_file,		/* Name of file containing is */
	_ins_character,	/* Insert character */
	_ins_line,		/* Add new blank line (*) */
	_ins_padding,		/* Insert pad after character inserted (*) */
	_ky_backspace,		/* KEY_BACKSPACE, 0407, Sent by backspace key */
	_ky_catab,		/* KEY_CATAB, 0526, Sent by clear-all-tabs key. */
	_ky_clear,		/* KEY_CLEAR, 0515, Sent by clear screen or erase key. */
	_ky_ctab,		/* KEY_CTAB, 0525, Sent by clear-tab key */
	_ky_dc,			/* KEY_DC, 0512, Sent by delete character key. */
	_ky_dl,			/* KEY_DL, 0510, Sent by delete line key. */
	_ky_down,		/* KEY_DOWN, 0402, Sent by terminal down arrow key */
	_ky_eic,		/* KEY_EIC, 0514, Sent by rmir or smir in insert mode. */
	_ky_eol,		/* KEY_EOL, 0517, Sent by clear-to-end-of-line key. */
	_ky_eos,		/* KEY_EOS, 0516, Sent by clear-to-end-of-screen key. */
	_ky_f0,			/* KEY_F(0), 0410, function key f0. */
	_ky_f1,			/* KEY_F(1), 0411, function key f1. */
	_ky_f10,		/* KEY_F(10), 0422, function key f10. */
	_ky_f2,			/* KEY_F(2), 0412, function key f2. */
	_ky_f3,			/* KEY_F(3), 0413, function key f3. */
	_ky_f4,			/* KEY_F(4), 0414, function key f4. */
	_ky_f5,			/* KEY_F(5), 0415, function key f5. */
	_ky_f6,			/* KEY_F(6), 0416, function key f6. */
	_ky_f7,			/* KEY_F(7), 0417, function key f7. */
	_ky_f8,			/* KEY_F(8), 0420, function key f8. */
	_ky_f9,			/* KEY_F(9), 0421, function key f9. */
	_ky_home,		/* KEY_HOME, 0406, Sent by home key. */
	_ky_ic,			/* KEY_IC, 0513, Sent by ins char/enter ins mode key. */
	_ky_il,			/* KEY_IL, 0511, Sent by insert line. */
	_ky_left,		/* KEY_LEFT, 0404, Sent by terminal left arrow key */
	_ky_ll,			/* KEY_LL, 0533, Sent by home-down key */
	_ky_npage,		/* KEY_NPAGE, 0522, Sent by next-page key */
	_ky_ppage,		/* KEY_PPAGE, 0523, Sent by previous-page key */
	_ky_right,		/* KEY_RIGHT, 0405, Sent by terminal right arrow key */
	_ky_sf,			/* KEY_SF, 0520, Sent by scroll-forward/down key */
	_ky_sr,			/* KEY_SR, 0521, Sent by scroll-backward/up key */
	_ky_stab,		/* KEY_STAB, 0524, Sent by set-tab key */
	_ky_up,			/* KEY_UP, 0403, Sent by terminal up arrow key */
	_kpad_local,		/* Out of "keypad transmit" mode */
	_kpad_xmit,		/* Put terminal in "keypad transmit" mode */
	_lab_f0,		/* Labels on function key f0 if not f0 */
	_lab_f1,		/* Labels on function key f1 if not f1 */
	_lab_f10,		/* Labels on function key f10 if not f10 */
	_lab_f2,		/* Labels on function key f2 if not f2 */
	_lab_f3,		/* Labels on function key f3 if not f3 */
	_lab_f4,		/* Labels on function key f4 if not f4 */
	_lab_f5,		/* Labels on function key f5 if not f5 */
	_lab_f6,		/* Labels on function key f6 if not f6 */
	_lab_f7,		/* Labels on function key f7 if not f7 */
	_lab_f8;			/* Labels on function key f8 if not f8 */
};

struct strs2 {
    charptr
	_lab_f9,		/* Labels on function key f9 if not f9 */
	_meta_off,		/* Turn off "meta mode" */
	_meta_on,		/* Turn on "meta mode" (8th bit) */
	_newline,		/* Newline (behaves like cr followed by lf) */
	_pad_char,		/* Pad character (rather than null) */
	_prm_dch,		/* Delete #1 chars (G*) */
	_prm_delete_line,	/* Delete #1 lines (G*) */
	_prm_down_cursor,	/* Move cursor down #1 lines. (G*) */
	_prm_ich,		/* Insert #1 blank chars (G*) */
	_prm_index,		/* Scroll forward #1 lines. (G) */
	_prm_insert_line,	/* Add #1 new blank lines (G*) */
	_prm_left_cursor,	/* Move cursor left #1 spaces (G) */
	_prm_right_cursor,	/* Move cursor right #1 spaces. (G*) */
	_prm_rindex,		/* Scroll backward #1 lines. (G) */
	_prm_up_cursor,		/* Move cursor up #1 lines. (G*) */
	_pkey_key,		/* Prog funct key #1 to type string #2 */
	_pkey_local,		/* Prog funct key #1 to execute string #2 */
	_pkey_xmit,		/* Prog funct key #1 to xmit string #2 */
	_print_screen,		/* Print contents of the screen */
	_prtr_off,		/* Turn off the printer */
	_prtr_on,		/* Turn on the printer */
	_repeat_char,		/* Repeat char #1 #2 times.  (G*) */
	_reset_1string,		/* Reset terminal completely to sane modes. */
	_reset_2string,		/* Reset terminal completely to sane modes. */
	_reset_3string,		/* Reset terminal completely to sane modes. */
	_reset_file,		/* Name of file containing reset string. */
	_restore_cursor,	/* Restore cursor to position of last sc. */
	_row_address,		/* Like hpa but sets row. (G) */
	_save_cursor,		/* Save cursor position. */
	_scrll_forward,		/* Scroll text up */
	_scrll_reverse,		/* Scroll text down */
	_set_attributes,	/* Define the video attributes (G9) */
	_set_tab,		/* Set a tab in all rows, current column. */
	_set_window,		/* Current window is lines #1-#2 cols #3-#4 */
	_tab,			/* Tab to next 8 space hardware tab stop. */
	_to_status_line,	/* Go to status line, col #1 */
	_underline_char,	/* Underscore one char and move past it */
	_up_half_line,		/* Half-line up (reverse 1/2 linefeed) */
	_init_prog,		/* Path name of program for init. */
	_ky_a1,			/* KEY_A1, 0534, Upper left of keypad */
	_ky_a3,			/* KEY_A3, 0535, Upper right of keypad */
	_ky_b2,			/* KEY_B2, 0536, Center of keypad */
	_ky_c1,			/* KEY_C1, 0537, Lower left of keypad */
	_ky_c3,			/* KEY_C3, 0540, Lower right of keypad */
	_prtr_non,		/* Turn on the printer for #1 bytes. */
	_char_padding,		/* Like ip but when in replace mode */
	_acs_chars,		/* Graphic charset pairs aAbBcC - def=vt100 */
	_plab_norm,		/* Prog label #1 to show string #2 */
	_ky_btab,		/* KEY_BTAB, 0541, Back tab key */
	_entr_xon_mode,		/* Turn on xon/xoff handshaking */
	_exit_xon_mode,		/* Turn off xon/xoff handshaking */
	_entr_am_mode,		/* Turn on automatic margins */
	_exit_am_mode,		/* Turn off automatic margins */
	_xon_character,		/* X-on character */
	_xoff_character,	/* X-off character */
	_ena_acs,		/* Enable alternate char set */
	_labl_on,		/* Turn on soft labels */
	_labl_off,		/* Turn off soft labels */
	_ky_beg,		/* KEY_BEG, 0542, beg(inning) key */
	_ky_cancel,		/* KEY_CANCEL, 0543, cancel key */
	_ky_close,		/* KEY_CLOSE, 0544, close key */
	_ky_command,		/* KEY_COMMAND, 0545, cmd (command) key */
	_ky_copy,		/* KEY_COPY, 0546, copy key */
	_ky_create,		/* KEY_CREATE, 0547, create key */
	_ky_end,		/* KEY_END, 0550, end key */
	_ky_enter,		/* KEY_ENTER, 0527, Enter/send (unreliable) */
	_ky_exit,		/* KEY_EXIT, 0551, exit key */
	_ky_find,		/* KEY_FIND, 0552, find key */
	_ky_help,		/* KEY_HELP, 0553, help key */
	_ky_mark,		/* KEY_MARK, 0554, mark key */
	_ky_message,		/* KEY_MESSAGE, 0555, message key */
	_ky_move,		/* KEY_MOVE, 0556, move key */
	_ky_next,		/* KEY_NEXT, 0557, next object key */
	_ky_open,		/* KEY_OPEN, 0560, open key */
	_ky_options,		/* KEY_OPTIONS, 0561, options key */
	_ky_previous,		/* KEY_PREVIOUS, 0562, previous object key */
	_ky_print,		/* KEY_PRINT, 0532, print or copy */
	_ky_redo,		/* KEY_REDO, 0563, redo key */
	_ky_reference,		/* KEY_REFERENCE, 0564, ref(erence) key */
	_ky_refresh,		/* KEY_REFRESH, 0565, refresh key */
	_ky_replace,		/* KEY_REPLACE, 0566, replace key */
	_ky_restart,		/* KEY_RESTART, 0567, restart key */
	_ky_resume,		/* KEY_RESUME, 0570, resume key */
	_ky_save,		/* KEY_SAVE, 0571, save key */
	_ky_suspend,		/* KEY_SUSPEND, 0627, suspend key */
	_ky_undo,		/* KEY_UNDO, 0630, undo key */
	_ky_sbeg,		/* KEY_SBEG, 0572, shifted beginning key */
	_ky_scancel,		/* KEY_SCANCEL, 0573, shifted cancel key */
	_ky_scommand,		/* KEY_SCOMMAND, 0574, shifted command key */
	_ky_scopy,		/* KEY_SCOPY, 0575, shifted copy key */
	_ky_screate,		/* KEY_SCREATE, 0576, shifted create key */
	_ky_sdc,		/* KEY_SDC, 0577, shifted delete char key */
	_ky_sdl,		/* KEY_SDL, 0600, shifted delete line key */
	_ky_select,		/* KEY_SELECT, 0601, select key */
	_ky_send,		/* KEY_SEND, 0602, shifted end key */
	_ky_seol,		/* KEY_SEOL, 0603, shifted clear line key */
	_ky_sexit,		/* KEY_SEXIT, 0604, shifted exit key */
	_ky_sfind,		/* KEY_SFIND, 0605, shifted find key */
	_ky_shelp,		/* KEY_SHELP, 0606, shifted help key */
	_ky_shome;		/* KEY_SHOME, 0607, shifted home key */
};

struct strs3 {
    charptr
	_ky_sic,		/* KEY_SIC, 0610, shifted input key */
	_ky_sleft,		/* KEY_SLEFT, 0611, shifted left arrow key */
	_ky_smessage,		/* KEY_SMESSAGE, 0612, shifted message key */
	_ky_smove,		/* KEY_SMOVE, 0613, shifted move key */
	_ky_snext,		/* KEY_SNEXT, 0614, shifted next key */
	_ky_soptions,		/* KEY_SOPTIONS, 0615, shifted options key */
	_ky_sprevious,		/* KEY_SPREVIOUS, 0616, shifted prev key */
	_ky_sprint,		/* KEY_SPRINT, 0617, shifted print key */
	_ky_sredo,		/* KEY_SREDO, 0620, shifted redo key */
	_ky_sreplace,		/* KEY_SREPLACE, 0621, shifted replace key */
	_ky_sright,		/* KEY_SRIGHT, 0622, shifted right arrow */
	_ky_srsume,		/* KEY_SRSUME, 0623, shifted resume key */
	_ky_ssave,		/* KEY_SSAVE, 0624, shifted save key */
	_ky_ssuspend,		/* KEY_SSUSPEND, 0625, shifted suspend key */
	_ky_sundo,		/* KEY_SUNDO, 0626, shifted undo key */
	_req_for_input,		/* send next input char (for ptys) */
	_ky_f11,		/* KEY_F(11), 0423, function key f11. */
	_ky_f12,		/* KEY_F(12), 0424, function key f12. */
	_ky_f13,		/* KEY_F(13), 0425, function key f13. */
	_ky_f14,		/* KEY_F(14), 0426, function key f14. */
	_ky_f15,		/* KEY_F(15), 0427, function key f15. */
	_ky_f16,		/* KEY_F(16), 0430, function key f16. */
	_ky_f17,		/* KEY_F(17), 0431, function key f17. */
	_ky_f18,		/* KEY_F(18), 0432, function key f18. */
	_ky_f19,		/* KEY_F(19), 0433, function key f19. */
	_ky_f20,		/* KEY_F(20), 0434, function key f20. */
	_ky_f21,		/* KEY_F(21), 0435, function key f21. */
	_ky_f22,		/* KEY_F(22), 0436, function key f22. */
	_ky_f23,		/* KEY_F(23), 0437, function key f23. */
	_ky_f24,		/* KEY_F(24), 0440, function key f24. */
	_ky_f25,		/* KEY_F(25), 0441, function key f25. */
	_ky_f26,		/* KEY_F(26), 0442, function key f26. */
	_ky_f27,		/* KEY_F(27), 0443, function key f27. */
	_ky_f28,		/* KEY_F(28), 0444, function key f28. */
	_ky_f29,		/* KEY_F(29), 0445, function key f29. */
	_ky_f30,		/* KEY_F(30), 0446, function key f30. */
	_ky_f31,		/* KEY_F(31), 0447, function key f31. */
	_ky_f32,		/* KEY_F(32), 0450, function key f32. */
	_ky_f33,		/* KEY_F(33), 0451, function key f33. */
	_ky_f34,		/* KEY_F(34), 0452, function key f34. */
	_ky_f35,		/* KEY_F(35), 0453, function key f35. */
	_ky_f36,		/* KEY_F(36), 0454, function key f36. */
	_ky_f37,		/* KEY_F(37), 0455, function key f37. */
	_ky_f38,		/* KEY_F(38), 0456, function key f38. */
	_ky_f39,		/* KEY_F(39), 0457, function key f39. */
	_ky_f40,		/* KEY_F(40), 0460, function key f40. */
	_ky_f41,		/* KEY_F(41), 0461, function key f41. */
	_ky_f42,		/* KEY_F(42), 0462, function key f42. */
	_ky_f43,		/* KEY_F(43), 0463, function key f43. */
	_ky_f44,		/* KEY_F(44), 0464, function key f44. */
	_ky_f45,		/* KEY_F(45), 0465, function key f45. */
	_ky_f46,		/* KEY_F(46), 0466, function key f46. */
	_ky_f47,		/* KEY_F(47), 0467, function key f47. */
	_ky_f48,		/* KEY_F(48), 0470, function key f48. */
	_ky_f49,		/* KEY_F(49), 0471, function key f49. */
	_ky_f50,		/* KEY_F(50), 0472, function key f50. */
	_ky_f51,		/* KEY_F(51), 0473, function key f51. */
	_ky_f52,		/* KEY_F(52), 0474, function key f52. */
	_ky_f53,		/* KEY_F(53), 0475, function key f53. */
	_ky_f54,		/* KEY_F(54), 0476, function key f54. */
	_ky_f55,		/* KEY_F(55), 0477, function key f55. */
	_ky_f56,		/* KEY_F(56), 0500, function key f56. */
	_ky_f57,		/* KEY_F(57), 0501, function key f57. */
	_ky_f58,		/* KEY_F(58), 0502, function key f58. */
	_ky_f59,		/* KEY_F(59), 0503, function key f59. */
	_ky_f60,		/* KEY_F(60), 0504, function key f60. */
	_ky_f61,		/* KEY_F(61), 0505, function key f61. */
	_ky_f62,		/* KEY_F(62), 0506, function key f62. */
	_ky_f63,		/* KEY_F(63), 0507, function key f63. */
	_clr_bol,		/* Clear to beginning of line, inclusive */
	_clear_margins,		/* Clear left and right soft margins */
	_set_left_margin,	/* Set soft left margin */
	_set_right_margin,	/* Set soft right margin */
	_labl_format,		/* Label format */
	_set_clock,		/* Set time-of-day clock */
	_display_clock,		/* Display time-of-day clock */
	_remove_clock,		/* Remove time-of-day clock */
	_create_window,		/* Define win #1 to go from #2,#3 to #4,#5 */
	_goto_window,		/* Got to window #1 */
	_hangup,		/* Hang-up phone */
	_dial_phone,		/* Dial phone number #1 */
	_quick_dial,		/* Dial phone number #1, without progress detection */
	_tone,			/* Select touch tone dialing */
	_pulse,			/* Select pulse dialing */
	_flash_hook,		/* Flash the switch hook */
	_fixed_pause,		/* Pause for 2-3 seconds */
	_wait_tone,		/* Wait for dial tone */
	_user0,			/* User string 0 */
	_user1,			/* User string 1 */
	_user2,			/* User string 2 */
	_user3,			/* User string 3 */
	_user4,			/* User string 4 */
	_user5,			/* User string 5 */
	_user6,			/* User string 6 */
	_user7,			/* User string 7 */
	_user8,			/* User string 8 */
	_user9,			/* User string 9 */
	Sentinel;		/* End of strings. DO NOT MOVE! */
};

struct _bool_struct {
    char
	_auto_left_margin,	/* cub1 wraps from column 0 to last column */
	_auto_right_margin,	/* Terminal has automatic margins */
	_no_esc_ctlc,		/* Beehive (f1=escape, f2=ctrl C) */
	_ceol_standout_glitch,	/* Standout not erased by overwriting (hp) */
	_eat_newline_glitch,	/* newline ignored after 80 cols (Concept) */
	_erase_overstrike,	/* Can erase overstrikes with a blank */
	_generic_type,		/* Generic line type (e.g. dialup, switch). */
	_hard_copy,		/* Hardcopy terminal */
	_has_meta_key,		/* Has a meta key (shift, sets parity bit) */
	_has_status_line,	/* Has extra "status line" */
	_ins_null_glitch,	/* Insert mode distinguishes nulls */
	_mem_above,		/* Display may be retained above the screen */
	_mem_below,		/* Display may be retained below the screen */
	_move_insert_mode,	/* Safe to move while in insert mode */
	_move_standout_mode,	/* Safe to move in standout modes */
	_over_strike,		/* Terminal overstrikes */
	_status_line_esc_ok,	/* Escape can be used on the status line */
	_dest_tabs_magic_smso,	/* Tabs destructive, magic so char (t1061) */
	_tilde_glitch,		/* Hazeltine; can't print ~'s */
	_transparent_underline,	/* underline character overstrikes */
	_xon_xoff,		/* Terminal uses xon/xoff handshaking */
	_needs_xon_xoff,	/* Padding won't work, xon/xoff required */
	_prtr_silent,		/* Printer won't echo on screen. */
	_hard_cursor,		/* Cursor is hard to see. */
	_non_rev_rmcup,		/* Smcup does not reverse rmcup. */
	_no_pad_char,		/* Pad character doesn't exist. */
	_non_dest_scroll_region,/* Scrolling region is non-destructive. */
	Sentinel;		/* End of bools. DO NOT MOVE! */
};

struct _num_struct {
    short
	_columns,		/* Number of columns in a line */
	_init_tabs,		/* Tabs initially every # spaces. */
	_lines,			/* Number of lines on screen or page */
	_lines_of_memory,	/* Lines of memory if > lines.  0 => varies */
	_magic_cookie_glitch,	/* Number blank chars left by smso or rmso */
	_padding_baud_rate,	/* Lowest baud rate where padding needed */
	_virtual_terminal,	/* Virtual terminal number (CB/Unix) */
	_width_status_line,	/* # columns in status line */
	_num_labels,		/* # of labels on screen (start at 1) */
	_labl_height,		/* # rows in each label */
	_labl_width,		/* # cols in each label */
	_max_attributes,	/* max combined video attributes terminal can display */
	_maximum_windows,	/* Maximum number of defineable windows */
	Sentinel;		/* End of nums. DO NOT MOVE! */
};

struct _str_struct {
	struct strs strs;
	struct strs2 strs2;
	struct strs3 strs3;
};

#define NUM_ATTRIBUTES	9
#define	UNACCESSIBLE		1
#define	NO_TERMINAL		2
#define CORRUPTED		3
#define	ENTRY_TOO_LONG		4
#define	TERMINFO_TOO_LONG	5
#define	TERM_BAD_MALLOC		6
#define NOT_READABLE		7
#define _VR2_COMPAT_CODE
#ifdef _VR2_COMPAT_CODE
struct _Vr2_Astrs {
    charptr
	_s1,  _s2,  _s3,  _s4,  _s5,  _s6,  _s7,  _s8,  _s9,  _s10,
	_s11, _s12, _s13, _s14, _s15, _s16, _s17, _s18, _s19, _s20,
	_s21, _s22, _s23, _s24, _s25, _s26, _s27, _s28, _s29, _s30,
	_s31, _s32, _s33, _s34, _s35, _s36, _s37, _s38, _s39, _s40,
	_s41, _s42, _s43, _s44, _s45, _s46, _s47, _s48, _s49, _s50,
	_s51, _s52, _s53, _s54, _s55, _s56, _s57, _s58, _s59, _s60,
	_s61, _s62, _s63, _s64, _s65, _s66, _s67, _s68, _s69, _s70,
	_s71, _s72, _s73, _s74, _s75, _s76, _s77, _s78, _s79, _s80,
	_s81, _s82, _s83, _s84, _s85, _s86, _s87, _s88, _s89, _s90,
	_s91, _s92, _s93, _s94, _s95, _s96, _s97, _s98, _s99, _s100;
};

struct _Vr2_Bstrs {
    charptr
	_s101, _s102, _s103, _s104, _s105, _s106, _s107, _s108, _s109, _s110,
	_s111, _s112, _s113, _s114, _s115, _s116, _s117, _s118, _s119, _s120,
	_s121, _s122, _s123, _s124, _s125, _s126, _s127, _s128, _s129, _s130,
	_s131, _s132, _s133, _s134, _s135, _s136, _s137, _s138, _s139, _s140,
	_s141, _s142, _s143, _s144, _s145;
};
#endif /* _VR2_COMPAT_CODE */

typedef struct {
	char *_sends;	/* Characters sent when key is pressed */
	short _keyval;	/* "char" we pass back to program */
} _KEY_MAP;

/*
 * This definition for the term struct allows the boolean, number
 * and string information to grow in the future and still allow .o
 * compatibility.
 */

struct term {
#ifdef _VR2_COMPAT_CODE
    char
	_b1,  _b2,  _b3,  _b4,  _b5,  _b6,  _b7,  _b8,  _b9,  _b10,
	_b11, _b12, _b13, _b14, _b15, _b16, _b17, _b18, _b19, _b20, _b21;
    short
	_c1, _c2, _c3, _c4, _c5, _c6, _c7, _c8;
	struct _Vr2_Astrs _Vr2_Astrs;
	struct _Vr2_Bstrs _Vr2_Bstrs;
#endif /* _VR2_COMPAT_CODE */
	short Filedes;		/* file descriptor being written to */
	SGTTY Ottyb,		/* original state of the terminal */
	      Nttyb;		/* current state of the terminal */
#ifdef DIOCSETT
	struct termcb new, old;	/* CB/UNIX virtual terminals */
#endif
#ifdef LTILDE
	int newlmode, oldlmode;	/* BSD tty driver */
#endif
	/* end of Vr2 structure */
	struct _bool_struct *_bools;
	struct _num_struct *_nums;
	struct _str_struct *_strs;
	char *_strtab;
#ifdef FIONREAD
	long timeout;		/* for BSD halfdelay mode */
#endif
	/* end of Vr3 structure */
	chtype sgr_mode;		/* current phys. graphic rendition */
	chtype sgr_faked;		/* attributes faked by vidputs */
	int    _delay;			/* timeout for inputs */
	int    _inputfd;		/* input file descriptor */
	int    _check_fd;		/* typeahead file descriptor */
	_KEY_MAP	**_keys,	/* key map */
			*internal_keys;	/* pointer to free key structures */
	short		_ksz,		/* size of keymap */
			_lastkey_ordered,/* where the last ordered key is */
			_lastmacro_ordered,/* where the last ordered macro is */
			_first_macro;
	bool				/* map of which chars start fn keys */
			funckeystarter[0400];	
	bool		_fl_rawmode,	/* in cbreak(=1) or raw(=2) mode */
			fl_typeahdok,	/* ok to use typeahead */
			_cursorstate,	/* cursor: 0=invis, 1=norm, 2=vvis */
			_iwait;		/* true if input-pending */
	short		_regs[26];	/* tparm static registers */
#define INP_QSIZE	32
	short				/* a place to put stuff ungetch'ed */
			_input_queue[INP_QSIZE],
			_ungotten;	/* # chars ungotten by ungetch() */
	char		_chars_on_queue; /* # chars on queue */
#ifdef _VR3_COMPAT_CODE
	_ochtype	*_acsmap;
	chtype		*_acs32map;	/* map of alternate char set chars */
#else /* _VR3_COMPAT_CODE */
	chtype		*_acsmap;	/* map of alternate char set chars */
#endif /* _VR3_COMPAT_CODE */
	char		*turn_on_seq[NUM_ATTRIBUTES];
	chtype		bit_vector;
	char		*cursor_seq[3];
	char		_termname[15];
	char		*turn_off_seq[3];
	chtype		check_turn_off;
	chtype		non_faked_mode;
};

typedef struct term TERMINAL;

/* allow old names for these capabilities */
#define beehive_glitch	no_esc_ctlc
#define teleray_glitch	dest_tabs_magic_smso

extern TERMINAL			*cur_term;
extern struct _bool_struct	*cur_bools;
extern struct _num_struct	*cur_nums;
extern struct _str_struct	*cur_strs;

#ifdef SINGLE
extern TERMINAL			_first_term;
extern struct _bool_struct	_frst_bools;
extern struct _num_struct	_frst_nums;
extern struct _str_struct	_frst_strs;

# define CUR			_first_term.
# define CURB			_frst_bools.
# define CURN			_frst_nums.
# define CURS			_frst_strs.
# define PROGTTY		_first_term.Nttyb
# define SHELLTTY		_first_term.Ottyb
#else
# define CUR			cur_term->
# define CURB			cur_bools->
# define CURN			cur_nums->
# define CURS			cur_strs->
# define PROGTTY		(cur_term->Nttyb)
# define SHELLTTY		(cur_term->Ottyb)
#endif
 
extern	chtype		termattrs();
extern	TERMINAL	*setcurterm(/* TERMINAL *newterminal */);

#ifdef	_VR3_COMPAT_CODE
extern	TERMINAL	*set_curterm(/* TERMINAL *newterminal */);
#endif	/* _VR3_COMPAT_CODE */

#ifndef NOMACROS
#define del_curterm	delterm
#define set_curterm	setcurterm
#endif /* NOMACROS */

extern	short	term_errno;

extern	char	*tparm(), *tgoto(), *tgetstr(), *tigetstr(),
		term_parm_err[], *term_err_strings[], *Def_term, *termname();
extern	char	*boolnames[], *boolcodes[], *boolfnames[],
		*numnames[], *numcodes[], *numfnames[],
		*strnames[], *strcodes[], *strfnames[];
extern	int	tputs(), putp(), vidputs(), vidattr();

extern	int	resetterm(), fixterm(), saveterm(), restartterm(), del_curterm(),
		delterm();

extern	void	termerr(), tinputfd();
#endif /* auto_left_margin */

#endif /* TERM_H */
