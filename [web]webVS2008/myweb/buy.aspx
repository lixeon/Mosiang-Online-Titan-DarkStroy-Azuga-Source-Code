<%@ Page language="c#" Codebehind="buy.aspx.cs" AutoEventWireup="false" Inherits="web.buy" %>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.0 Transitional//EN" >
<HTML>
	<HEAD>
		<title>購買物品</title>
		<meta content="Microsoft Visual Studio .NET 7.1" name="GENERATOR">
		<meta content="C#" name="CODE_LANGUAGE">
		<meta content="JavaScript" name="vs_defaultClientScript">
		<meta content="http://schemas.microsoft.com/intellisense/ie5" name="vs_targetSchema">
	</HEAD>
	<body MS_POSITIONING="GridLayout">
		<form id="Form1" method="post" runat="server">
			<!--css-->
			<style type="text/css">BODY { FONT-SIZE: 12px; BACKGROUND: #fff; FONT-FAMILY: Tahoma }
	H1 { PADDING-RIGHT: 0px; PADDING-LEFT: 0px; PADDING-BOTTOM: 0px; MARGIN: 0px; PADDING-TOP: 0px }
	H2 { PADDING-RIGHT: 0px; PADDING-LEFT: 0px; PADDING-BOTTOM: 0px; MARGIN: 0px; PADDING-TOP: 0px }
	H3 { PADDING-RIGHT: 0px; PADDING-LEFT: 0px; PADDING-BOTTOM: 0px; MARGIN: 0px; PADDING-TOP: 0px }
	H4 { PADDING-RIGHT: 0px; PADDING-LEFT: 0px; PADDING-BOTTOM: 0px; MARGIN: 0px; PADDING-TOP: 0px }
	H5 { PADDING-RIGHT: 0px; PADDING-LEFT: 0px; PADDING-BOTTOM: 0px; MARGIN: 0px; PADDING-TOP: 0px }
	H6 { PADDING-RIGHT: 0px; PADDING-LEFT: 0px; PADDING-BOTTOM: 0px; MARGIN: 0px; PADDING-TOP: 0px }
	FORM { PADDING-RIGHT: 0px; PADDING-LEFT: 0px; PADDING-BOTTOM: 0px; MARGIN: 0px; PADDING-TOP: 0px }
	BODY { PADDING-RIGHT: 0px; PADDING-LEFT: 0px; PADDING-BOTTOM: 0px; MARGIN: 0px; PADDING-TOP: 0px }
	TD { WORD-BREAK: break-all; WORD-WRAP: break-word }
	TH { WORD-BREAK: break-all; WORD-WRAP: break-word }
	DIV { WORD-BREAK: break-all; WORD-WRAP: break-word }
	TABLE { empty-cells: show }
	IMG { BORDER-RIGHT: 0px; BORDER-TOP: 0px; BORDER-LEFT: 0px; BORDER-BOTTOM: 0px }
	H3 { DISPLAY: inline; FONT-SIZE: 1em }
	H2 { DISPLAY: inline; FONT-SIZE: 1em }
	H3 { FONT-WEIGHT: normal }
	H2 A { COLOR: #000 }
	H3 A { COLOR: #000 }
	H4 { FONT-SIZE: 1.1em; MARGIN: 20px 0px 10px }
	TEXTAREA { PADDING-RIGHT: 3px; PADDING-LEFT: 3px; MARGIN-BOTTOM: 1px; PADDING-BOTTOM: 0px; FONT: 12px Arial; VERTICAL-ALIGN: middle; PADDING-TOP: 1px }
	INPUT { PADDING-RIGHT: 3px; PADDING-LEFT: 3px; MARGIN-BOTTOM: 1px; PADDING-BOTTOM: 0px; FONT: 12px Arial; VERTICAL-ALIGN: middle; PADDING-TOP: 1px }
	SELECT { PADDING-RIGHT: 3px; PADDING-LEFT: 3px; MARGIN-BOTTOM: 1px; PADDING-BOTTOM: 0px; FONT: 12px Arial; VERTICAL-ALIGN: middle; PADDING-TOP: 1px }
	.c { CLEAR: both; FONT: 0px/0 Arial; HEIGHT: 0px }
	.b { FONT-WEIGHT: bold }
	.tal { TEXT-ALIGN: left }
	.tac { TEXT-ALIGN: center }
	.tar { TEXT-ALIGN: right }
	.fr { FLOAT: right }
	.fl { FLOAT: left }
	A { COLOR: #2f5fa1; TEXT-DECORATION: none }
	A:hover { TEXT-DECORATION: underline }
	.f9 { FONT-SIZE: 9px }
	.f10 { FONT-SIZE: 11px }
	.f12 { FONT-SIZE: 12px }
	.f14 { FONT-SIZE: 14px }
	.fn { FONT-WEIGHT: normal }
	.fn A { FONT-WEIGHT: normal }
	.s1 { COLOR: #008000 }
	.s2 { COLOR: #984b98 }
	.s3 { COLOR: #fa891b }
	.s4 { COLOR: #0033ff }
	.s5 { COLOR: #659b28 }
	.gray { COLOR: #818a89 }
	.f_one { BACKGROUND: #ffffff }
	.t_one { BACKGROUND: #ffffff }
	.r_one { BACKGROUND: #ffffff }
	.f_two { BACKGROUND: #f4fbff }
	.t_two { BACKGROUND: #f4fbff }
	.r_two { BACKGROUND: #f4fbff }
	TEXTAREA { PADDING-RIGHT: 3px; PADDING-LEFT: 3px; MARGIN-BOTTOM: 1px; PADDING-BOTTOM: 0px; FONT: 12px Arial; VERTICAL-ALIGN: middle; PADDING-TOP: 1px }
	INPUT { PADDING-RIGHT: 3px; PADDING-LEFT: 3px; MARGIN-BOTTOM: 1px; PADDING-BOTTOM: 0px; FONT: 12px Arial; VERTICAL-ALIGN: middle; PADDING-TOP: 1px }
	SELECT { PADDING-RIGHT: 3px; PADDING-LEFT: 3px; MARGIN-BOTTOM: 1px; PADDING-BOTTOM: 0px; FONT: 12px Arial; VERTICAL-ALIGN: middle; PADDING-TOP: 1px }
	SELECT { BORDER-RIGHT: #d4eff7 1px solid; BORDER-TOP: #d4eff7 1px solid; BORDER-LEFT: #d4eff7 1px solid; BORDER-BOTTOM: #d4eff7 1px solid }
	.btn { BORDER-TOP-WIDTH: 1px; PADDING-RIGHT: 15px; PADDING-LEFT: 15px; BORDER-LEFT-WIDTH: 1px; BACKGROUND: #a6cbe7; BORDER-BOTTOM-WIDTH: 1px; VERTICAL-ALIGN: middle; COLOR: #004c7d; BORDER-RIGHT-WIDTH: 1px }
	.input { BORDER-RIGHT: #a6cbe7 1px solid; PADDING-RIGHT: 0px; BORDER-TOP: #a6cbe7 1px solid; PADDING-LEFT: 1px; FONT-SIZE: 1em; PADDING-BOTTOM: 2px; VERTICAL-ALIGN: middle; BORDER-LEFT: #a6cbe7 1px solid; PADDING-TOP: 2px; BORDER-BOTTOM: #a6cbe7 1px solid }
	FORM { DISPLAY: inline }
	TEXTAREA { BORDER-RIGHT: #a6cbe7 1px solid; BORDER-TOP: #a6cbe7 1px solid; BORDER-LEFT: #a6cbe7 1px solid; BORDER-BOTTOM: #a6cbe7 1px solid }
	#header { MARGIN: auto; WIDTH: 98% }
	.toptool { BORDER-BOTTOM: #daebcd 1px solid }
	.toptool SPAN { PADDING-RIGHT: 5px; PADDING-LEFT: 5px; PADDING-BOTTOM: 1px; LINE-HEIGHT: 150%; PADDING-TOP: 1px }
	.banner { PADDING-RIGHT: 3% }
	.banner IMG { VERTICAL-ALIGN: middle }
	.guide { }
	.t { BORDER-RIGHT: #a6cbe7 1px solid; BORDER-TOP: #a6cbe7 1px solid; MARGIN: 0px auto 8px; BORDER-LEFT: #a6cbe7 1px solid; BORDER-BOTTOM: #a6cbe7 1px solid }
	.t TABLE { BORDER-RIGHT: #fff 1px solid; BORDER-TOP: #fff 1px solid; MARGIN: 0px auto; BORDER-LEFT: #fff 1px solid; WIDTH: 99.98%; BORDER-BOTTOM: #fff 1px solid }
	.t2 { BORDER-TOP: #a6cbe7 1px solid; MARGIN: 0px auto 5px }
	.t3 { MARGIN: auto }
	.t4 { PADDING-RIGHT: 0px; PADDING-LEFT: 1px; PADDING-BOTTOM: 1px; PADDING-TOP: 1px }
	.h { PADDING-RIGHT: 7px; PADDING-LEFT: 7px; BACKGROUND: #e0f0f9; PADDING-BOTTOM: 3px; COLOR: #004c7d; PADDING-TOP: 5px; BORDER-BOTTOM: #dbecf4 4px solid; TEXT-ALIGN: left }
	.h SPAN { FONT-WEIGHT: normal }
	.h H2 { FONT-WEIGHT: bold }
	.h A { COLOR: #004c7d; FONT-FAMILY: Arial }
	.h SPAN A { COLOR: #5599bb }
	.h SPAN { COLOR: #5599bb }
	.h A.a2 { MARGIN-LEFT: 12px }
	.tr1 TH { PADDING-RIGHT: 10px; PADDING-LEFT: 10px; FONT-WEIGHT: normal; PADDING-BOTTOM: 5px; VERTICAL-ALIGN: top; PADDING-TOP: 5px; TEXT-ALIGN: left }
	.tr1 TD.td1 { BORDER-RIGHT: #d4eff7 1px solid; BORDER-TOP: #d4eff7 1px solid; BORDER-LEFT: #d4eff7 1px solid; BORDER-BOTTOM: #d4eff7 1px solid }
	.tr2 { BACKGROUND: #f3f8ef; COLOR: #659b28 }
	.tr2 TD { PADDING-RIGHT: 6px; BORDER-TOP: #a6cbe7 1px solid; PADDING-LEFT: 6px; PADDING-BOTTOM: 0px; LINE-HEIGHT: 21px; PADDING-TOP: 0px; BORDER-BOTTOM: #daebcd 1px solid }
	.tr2 TH { PADDING-RIGHT: 6px; BORDER-TOP: #a6cbe7 1px solid; PADDING-LEFT: 6px; PADDING-BOTTOM: 0px; LINE-HEIGHT: 21px; PADDING-TOP: 0px; BORDER-BOTTOM: #daebcd 1px solid }
	.tr2 A { MARGIN: 2px; COLOR: #659b28 }
	.tr3 TD { PADDING-RIGHT: 8px; PADDING-LEFT: 8px; PADDING-BOTTOM: 5px; PADDING-TOP: 5px; BORDER-BOTTOM: #d4eff7 1px solid }
	.tr3 TH { PADDING-RIGHT: 8px; PADDING-LEFT: 8px; PADDING-BOTTOM: 5px; PADDING-TOP: 5px; BORDER-BOTTOM: #d4eff7 1px solid }
	.tr3 TH { FONT-WEIGHT: normal; TEXT-ALIGN: left }
	.tr4 { BACKGROUND: #e0f0f9; COLOR: #004c7d }
	.tr4 TD { PADDING-RIGHT: 10px; PADDING-LEFT: 10px; PADDING-BOTTOM: 4px; PADDING-TOP: 4px }
	.tr TD { PADDING-RIGHT: 2px; PADDING-LEFT: 2px; PADDING-BOTTOM: 2px; PADDING-TOP: 2px }
	.tr TH { PADDING-RIGHT: 2px; PADDING-LEFT: 2px; PADDING-BOTTOM: 2px; PADDING-TOP: 2px }
	.tpc_content { PADDING-RIGHT: 2%; PADDING-LEFT: 0.5%; FONT-SIZE: 14px; PADDING-BOTTOM: 0px; MARGIN: 0px 0px 2%; PADDING-TOP: 0px; FONT-FAMILY: Arial }
	.tips { BORDER-RIGHT: #d4eff7 1px solid; PADDING-RIGHT: 5px; BORDER-TOP: #d4eff7 1px solid; PADDING-LEFT: 5px; BACKGROUND: #f4fbff; FLOAT: left; PADDING-BOTTOM: 5px; MARGIN: 0px 1% 1% 0px; BORDER-LEFT: #d4eff7 1px solid; PADDING-TOP: 5px; BORDER-BOTTOM: #d4eff7 1px solid; TEXT-ALIGN: center }
	.tiptop { PADDING-RIGHT: 0px; PADDING-LEFT: 0px; PADDING-BOTTOM: 5px; VERTICAL-ALIGN: middle; PADDING-TOP: 0px; BORDER-BOTTOM: #d4eff7 1px solid }
	.tipad { PADDING-RIGHT: 0px; BORDER-TOP: #d4eff7 1px solid; PADDING-LEFT: 0px; PADDING-BOTTOM: 0px; MARGIN: 10px 0px 0px; PADDING-TOP: 5px }
	.quote { PADDING-RIGHT: 0px; PADDING-LEFT: 0px; FONT-SIZE: 70%; PADDING-BOTTOM: 0px; MARGIN: 2px; COLOR: #004c7d; PADDING-TOP: 0px }
	BLOCKQUOTE { BORDER-RIGHT: #d4eff7 1px solid; PADDING-RIGHT: 5px; BORDER-TOP: #d4eff7 1px solid; PADDING-LEFT: 5px; FONT-SIZE: 85%; PADDING-BOTTOM: 5px; MARGIN: 0px 0px 1%; BORDER-LEFT: #d4eff7 3px solid; WIDTH: 92%; COLOR: #81888c; PADDING-TOP: 5px; BORDER-BOTTOM: #d4eff7 1px solid }
	.menu { BORDER-RIGHT: #a6cbe7 1px solid; BORDER-TOP: #a6cbe7 1px solid; BACKGROUND: #fff; BORDER-LEFT: #a6cbe7 1px solid; BORDER-BOTTOM: #a6cbe7 1px solid; POSITION: absolute }
	.menu TD { PADDING-RIGHT: 0px; PADDING-LEFT: 0px; BACKGROUND: #e0f0f9; PADDING-BOTTOM: 0px; MARGIN: 0px; PADDING-TOP: 0px }
	.menu LI { PADDING-RIGHT: 0px; PADDING-LEFT: 0px; BACKGROUND: #e0f0f9; PADDING-BOTTOM: 0px; MARGIN: 0px; PADDING-TOP: 0px }
	.menu UL { PADDING-RIGHT: 0px; PADDING-LEFT: 0px; BACKGROUND: #e0f0f9; PADDING-BOTTOM: 0px; MARGIN: 0px; PADDING-TOP: 0px }
	.menu LI { LIST-STYLE-TYPE: none }
	.menu A { PADDING-RIGHT: 15px; DISPLAY: block; PADDING-LEFT: 15px; PADDING-BOTTOM: 3px; PADDING-TOP: 3px }
	.menu A:hover { BACKGROUND: #2f5fa1; COLOR: #fff; TEXT-DECORATION: none }
	.menu UL.ul1 LI A { PADDING-RIGHT: 0px; DISPLAY: inline; PADDING-LEFT: 0px; PADDING-BOTTOM: 0px; PADDING-TOP: 0px }
	.pages { MARGIN: 3px 0px; FONT: 11px/12px Tahoma }
	.pages { VERTICAL-ALIGN: middle }
	.pages A { BORDER-RIGHT: #a6cbe7 1px solid; PADDING-RIGHT: 4px; BORDER-TOP: #a6cbe7 1px solid; PADDING-LEFT: 4px; PADDING-BOTTOM: 1px; MARGIN: 0px 1px 0px 0px; FONT: 12px/14px verdana; BORDER-LEFT: #a6cbe7 1px solid; PADDING-TOP: 1px; BORDER-BOTTOM: #a6cbe7 1px solid; TEXT-ALIGN: center; TEXT-DECORATION: none }
	.pages A:hover { BORDER-RIGHT: #659b28 1px solid; BORDER-TOP: #659b28 1px solid; BACKGROUND: #f3f8ef; BORDER-LEFT: #659b28 1px solid; COLOR: #004c7d; BORDER-BOTTOM: #659b28 1px solid; TEXT-DECORATION: none }
	.pages INPUT { BORDER-RIGHT: #659b28 1px solid; BORDER-TOP: #659b28 1px solid; PADDING-LEFT: 1px; MARGIN-BOTTOM: 0px; PADDING-BOTTOM: 1px; FONT: bold 12px/15px Verdana; BORDER-LEFT: #659b28 1px solid; COLOR: #659b28; MARGIN-RIGHT: 1px; BORDER-BOTTOM: #659b28 1px solid; HEIGHT: 15px }
	#footer { PADDING-RIGHT: 0px; BORDER-TOP: #dbecf4 2px solid; PADDING-LEFT: 0px; PADDING-BOTTOM: 5px; MARGIN: auto; WIDTH: 98%; PADDING-TOP: 5px; BORDER-BOTTOM: #f3f8ef 12px solid; TEXT-ALIGN: right }
	#main { MARGIN: auto; WIDTH: 98% }
			</style> <!--css-->
			<style type="text/css">.tr3 TD { BORDER-RIGHT: #d4eff7 1px solid }
	.tr3 TH { BORDER-RIGHT: #d4eff7 1px solid }
	.y-style { TEXT-ALIGN: center }
	.t TABLE { BORDER-RIGHT: 0px; BORDER-TOP: 0px; BORDER-LEFT: 0px; WIDTH: 100%; BORDER-BOTTOM: 0px }
	.tr1 TH { BORDER-RIGHT: #d4eff7 1px solid }
	.tr1 TD.td1 { BORDER-LEFT: 0px }
	.t { PADDING-RIGHT: 1px; PADDING-LEFT: 1px; PADDING-BOTTOM: 1px; PADDING-TOP: 1px }
			</style>
			<div id="header">
				<div class="tr2 toptool tar"></div>
			</div>
			<div id="main">
				<div class="t3"></div>
				<div class="t3">
					<table cellSpacing="0" cellPadding="3" width="100%" align="center">
						<tr>
							<td vAlign="top">
								<div class="t" style="WIDTH: 98%; MARGIN-RIGHT: 0px">
													<table cellSpacing="0" cellPadding="0" width="100%" align="center">
										<tr>
											<td class="h" colSpan="6"><b>購物確認表單</b></td>
										</tr>
										<tr class="tr2">
											<td colSpan="6"><a>您是<span style="FONT-WEIGHT:bold;COLOR:#5D51F1"><%=vip%></span>,現在持有金幣數為<span style=FONT-WEIGHT:bold;COLOR:#ff3333><%=Session["webgold"]%></span>
													個 </a>
											</td>
										</tr>
										<tr class="tr1">
											<th style="WIDTH: 109px" width="109">
												<img src=<%=pic%> alt=<%=name%>></th>
											<th width="82%" height="96">
												<p>物品名稱：<span style="FONT-WEIGHT:bold;COLOR:#5D51F1"><%=name%></span></p>
												<p>物品說明：<%=comment%></p>
												<p>普通會員價格：<b>遊戲幣</b><span style=FONT-WEIGHT:bold;COLOR:#ff3333><%=iprice%></span><b>元</b>
													,<b>金幣</b><span style=FONT-WEIGHT:bold;COLOR:#ff3333><%=igold%></span><b>個</b>
													</p>
												<p>黃金會員價格：<b>遊戲幣</b><span style=FONT-WEIGHT:bold;COLOR:#ff3333><%=priceoffer%></span><b>元</b>
													,<b>金幣</b><span style=FONT-WEIGHT:bold;COLOR:#ff3333><%=goldoffer%></span><b>個</b></p>
												
											</th>
										</tr>
									</table>
								</div>

									<div class="t" style="WIDTH: 98%; MARGIN-RIGHT: 0px">
										<table cellSpacing="0" cellPadding="0" width="100%" align="center">
											<tr>
												<td class="h" colSpan="2"><b>選擇角色(確認已經退出遊戲)</b></td>
											</tr>
											<tr>
												<td style="PADDING-RIGHT: 0px; PADDING-LEFT: 5px; PADDING-BOTTOM: 2px; PADDING-TOP: 2px"
													align="right" width="17%">角色名：</td>
												<td style="PADDING-RIGHT: 0px; PADDING-LEFT: 5px; PADDING-BOTTOM: 2px; PADDING-TOP: 2px"
													width="83%">&nbsp;
													<asp:dropdownlist id="DropDownList1" runat="server"></asp:dropdownlist></td>
											</tr>
										</table>
									</div>
									<center><input class="btn" type="submit" value="我要購買(提示：百寶物品系統將會放在百寶倉庫中)" id="Submit1" name="Submit1" runat="server">
										<input class="btn" onclick="window.opener=null;window.close()" type="submit" value="離開"></center>
								</form>
							</td>
						</tr>
					</table>
				</div>
			</div>
		</form>
	</body>
</HTML>
