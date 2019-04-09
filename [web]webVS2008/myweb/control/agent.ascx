<%@ Control Language="c#" AutoEventWireup="false" Codebehind="agent.ascx.cs" Inherits="web.control.agent" TargetSchema="http://schemas.microsoft.com/intellisense/ie5"%>
<TABLE id="Table1" cellSpacing="0" cellPadding="0" width="96%" border="0">
	<TR>
		<TD height="30" style="WIDTH: 94px"></TD>
		<TD align="center" height="30"><SPAN id="AscxNewsView1_L_Title" style="FONT-WEIGHT: bold"><%=nation%>代理聯繫方式</SPAN></TD>
	</TR>
	<TR>
		<TD style="WIDTH: 94px; HEIGHT: 10px"><FONT face="宋体">姓名：</FONT></TD>
		<TD style="BACKGROUND-POSITION: 50% bottom; BACKGROUND-ATTACHMENT: scroll; BACKGROUND-REPEAT: no-repeat; HEIGHT: 10px"
			vAlign="top" height="10">
			<b>
				<%=name%>
			</b>
		</TD>
	</TR>
	<TR>
		<TD style="WIDTH: 94px; HEIGHT: 19px"><FONT face="宋体">手機：</FONT></TD>
		<TD style="BACKGROUND-POSITION: 50% bottom; BACKGROUND-ATTACHMENT: scroll; BACKGROUND-REPEAT: no-repeat; HEIGHT: 19px"
			vAlign="top" height="19"><%=hp%></TD>
	</TR>
	<TR>
		<TD style="WIDTH: 94px; HEIGHT: 19px"><FONT face="宋体">固定電話：</FONT></TD>
		<TD style="BACKGROUND-POSITION: 50% bottom; BACKGROUND-ATTACHMENT: scroll; BACKGROUND-REPEAT: no-repeat; HEIGHT: 19px"
			vAlign="top" height="19"><%=tp%><FONT face="宋体"></FONT></TD>
	</TR>
	<TR>
		<TD style="WIDTH: 94px; HEIGHT: 19px">MSN：</TD>
		<TD style="BACKGROUND-POSITION: 50% bottom; BACKGROUND-ATTACHMENT: scroll; BACKGROUND-REPEAT: no-repeat; HEIGHT: 19px"
			vAlign="top" height="19"><%=msn%></TD>
	</TR>
	<TR>
		<TD style="WIDTH: 94px; HEIGHT: 19px">QQ：</TD>
		<TD style="BACKGROUND-POSITION: 50% bottom; BACKGROUND-ATTACHMENT: scroll; BACKGROUND-REPEAT: no-repeat; HEIGHT: 19px"
			vAlign="top" height="19"><%=qq%></TD>
	</TR>
	<TR>
		<TD style="WIDTH: 94px; HEIGHT: 120px">銀行帳戶：</TD>
		<TD style="BACKGROUND-POSITION: 50% bottom; BACKGROUND-ATTACHMENT: scroll; BACKGROUND-REPEAT: no-repeat; HEIGHT: 120px"
			vAlign="top" height="120">
			<asp:TextBox id="TextBox1" runat="server" TextMode="MultiLine" Width="320px" Height="112px" ReadOnly="True"></asp:TextBox></TD>
	</TR>
	<TR>
		<TD style="WIDTH: 94px; HEIGHT: 76px">其它：</TD>
		<TD style="BACKGROUND-POSITION: 50% bottom; BACKGROUND-ATTACHMENT: scroll; BACKGROUND-REPEAT: no-repeat; HEIGHT: 76px"
			vAlign="top" height="76">
			<asp:TextBox id="TextBox2" runat="server" TextMode="MultiLine" Width="320px" Height="112px" ReadOnly="True"></asp:TextBox></TD>
	</TR>
	<TR>
		<TD style="WIDTH: 94px; HEIGHT: 19px"><FONT face="宋体">庫存金幣：</FONT></TD>
		<TD style="BACKGROUND-POSITION: 50% bottom; BACKGROUND-ATTACHMENT: scroll; BACKGROUND-REPEAT: no-repeat; HEIGHT: 19px"
			vAlign="top" height="19"><font color="#ff3333"><b>充足</b></font></TD>
	</TR>
</TABLE>
