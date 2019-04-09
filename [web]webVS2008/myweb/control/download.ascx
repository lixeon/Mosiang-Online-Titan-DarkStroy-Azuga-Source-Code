<%@ Control Language="c#" AutoEventWireup="false" Codebehind="download.ascx.cs" Inherits="web.control.download" TargetSchema="http://schemas.microsoft.com/intellisense/ie5"%>
<%@ OutputCache Duration="600" VaryByParam="type" %>
<asp:DataGrid id="DataGrid1" runat="server" GridLines="None" ShowHeader="False"
 AutoGenerateColumns="False" Width=96%>
	<Columns>
		<asp:TemplateColumn>
			<ItemTemplate>
				<TABLE cellSpacing="0" cellPadding="0" width="100%" border="0">
					<TR height="22">
						<TD vAlign="bottom">&nbsp;<img src="images/icon_leftmenu_purple.gif" align="absMiddle">
							<a href=<%# DataBinder.Eval(Container.DataItem,"link")%>><span id="AscxDownload1_DataList1__ctl0_Label1" style="FONT-WEIGHT:bold;COLOR:#ff8000">
									<%# DataBinder.Eval(Container.DataItem,"name")%></span></a></TD>
					</TR>
					<TR>
						<TD height="26"><TABLE height="100%" cellSpacing="0" cellPadding="0" width="100%" border="0">
								<TR>
									<TD style="PADDING-RIGHT: 20px; PADDING-LEFT: 10px; LINE-HEIGHT: 18px; LETTER-SPACING: -0.15mm"
										width="376"><FONT face="細明體"> <span id="AscxDownload1_DataList1__ctl0_Label2"><%# DataBinder.Eval(Container.DataItem,"comment")%></span></FONT></TD>
									<TD style="BACKGROUND-POSITION: center center; PADDING-LEFT: 33px; BACKGROUND-IMAGE: url(images/down2.gif); COLOR: #000000; BACKGROUND-REPEAT: no-repeat"
										width="121" align="center">&nbsp; <a id="AscxDownload1_DataList1__ctl0_HyperLink1" href=<%# DataBinder.Eval(Container.DataItem,"link")%>
											target="_blank"><strong>[點擊下載]</strong></a>&nbsp;&nbsp;</TD>
								</TR>
							</TABLE>
						</TD>
					</TR>
					<TR height="24">
						<TD><FONT face="細明體">&nbsp;&nbsp;&nbsp;&nbsp; <span id="AscxDownload1_DataList1__ctl0_Label4" style="FONT-WEIGHT:bold">
									<%# DataBinder.Eval(Container.DataItem,"date")%></span><STRONG>更新</STRONG></FONT></TD>
					</TR>
				</TABLE>
				<TR>
						<TD height="2">
							<HR width="99%" color="#000000">
						</TD>
					</TR>
			</ItemTemplate>
		</asp:TemplateColumn>
	</Columns>
</asp:DataGrid>
