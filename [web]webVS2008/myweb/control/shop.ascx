<%@ Control Language="c#" AutoEventWireup="false" Codebehind="shop.ascx.cs" Inherits="web.control.shop" TargetSchema="http://schemas.microsoft.com/intellisense/ie5"%>
<table>
	<TR>
		<TD align="center">
			<P align="center">查找物品：
				<asp:textbox id="tbitemname" runat="server"></asp:textbox>&nbsp;
				<asp:button id="btnsearch" runat="server" Text="模糊查找"></asp:button>&nbsp;&nbsp;&nbsp;<BR><BR>
			</P>
		</TD>
	</TR>
</table>
				<asp:DataList id="DataList1" runat="server" ShowHeader="False" ShowFooter="False" RepeatDirection="Horizontal"
					BorderColor="#DEBA84" BorderStyle="None" BackColor="#DEBA84" CellPadding="3"
					GridLines="Both" BorderWidth="1px" CellSpacing="2" >
					<SelectedItemStyle Font-Bold="True" ForeColor="White" BackColor="#738A9C"></SelectedItemStyle>
					<AlternatingItemStyle BackColor="White"></AlternatingItemStyle>
					<ItemStyle ForeColor="#8C4510" BackColor="#FFF7E7"></ItemStyle>
					<FooterStyle BackColor="#CCCC99"></FooterStyle>
					<ItemTemplate>
						<a href=shop.aspx?type=<%# DataBinder.Eval(Container.DataItem, "id") %>><b><%# DataBinder.Eval(Container.DataItem, "name") %></b></a>
					</ItemTemplate>
				</asp:DataList>

<table>
<tr align="Left">
<td>
溫馨提示
<FONT color="blue">請退出遊戲後再來購買物品</FONT>										
</td>
</tr>
<hr>
<tr><td>
<asp:DataGrid id="DataGrid1" runat="server" AutoGenerateColumns="False" AllowPaging="True" PageSize="60">	
	<Columns>
		<asp:TemplateColumn>
			<ItemTemplate>
				<table border="0" cellpadding="0" cellspacing="0" width="100%">
					<tr style="COLOR:#003399;BACKGROUND-COLOR:white">
						<td>
							<TABLE style="COLOR: #000000" cellSpacing="0" cellPadding="0" width="100%" border="0">
								<TR>
									<TD align="center" width="100" rowSpan="3"><IMG height=80 src='<%# DataBinder.Eval(Container.DataItem,"pic")%>' width=80 align=absMiddle alt='<%# DataBinder.Eval(Container.DataItem,"name")%>'></TD>
									<TD width="60">物品名稱</TD>
									<TD width="300" align=left>
										<span style="FONT-WEIGHT:bold;COLOR:#5D51F1">
											<%# DataBinder.Eval(Container.DataItem,"name")%>
										</span>
									</TD>
									<TD width=200>需要金幣: <span><font color="#ff3333"><b>
													<%# DataBinder.Eval(Container.DataItem,"gold")%>
												</b></font>個</span></TD>
								</TR>
								<TR>
									<TD>物品類型:</TD>
									<TD><%# DataBinder.Eval(Container.DataItem,"type")%>[<%# DataBinder.Eval(Container.DataItem,"isbb")%>]</SPAN></TD>
									<td>需要遊戲幣: <span style=FONT-WEIGHT:bold;COLOR:#ff3333>
											<%# DataBinder.Eval(Container.DataItem,"price")%>
										</span>
									</td>
									<TD align=center><img onClick="javascript:window.open('buy.aspx?ID=<%# DataBinder.Eval(Container.DataItem,"id")%>','_blank')"
											src=images/bg_btn_buy.gif style="CURSOR: hand"></TD>
								</TR>
								<TR>
									<TD>物品說明</TD>
									<TD colSpan=3>
										<span>
											<%# DataBinder.Eval(Container.DataItem,"comment")%>
										</span>
									</TD>
								</TR>
							
							</TABLE>
						</td>
					</tr>
				</table>
		
			</ItemTemplate>
		</asp:TemplateColumn>
	</Columns>
	<PagerStyle BackColor=Maroon Font-Bold=True HorizontalAlign=Center ForeColor=#ffffff Mode="NumericPages" ></PagerStyle>
</asp:DataGrid>
</td></tr>
</table>