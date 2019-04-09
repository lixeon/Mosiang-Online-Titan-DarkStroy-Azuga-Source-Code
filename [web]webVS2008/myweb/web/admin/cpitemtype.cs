namespace web.admin
{
    using System;
    using System.Web.UI;
    using System.Web.UI.WebControls;
    using web;

    public class cpitemtype : Page
    {
        protected Button btnadd;
        protected DataGrid DataGrid1;
        protected RadioButton rbno;
        protected RadioButton rbyes;
        protected TextBox tbisbb;
        protected TextBox tbname;

        private void btnadd_Click(object sender, EventArgs e)
        {
            if (this.tbname.Text.ToString() == "")
            {
                base.Response.Write("<script language=javascript>alert(\"類型名稱不能為空\")</script>");
            }
            else
            {
                int num = this.rbyes.Checked ? 1 : 0;
                int num2 = int.Parse(this.tbisbb.Text.ToString());
                new DataProviders().ExecuteSql(string.Concat(new object[] { "insert into web_itemtype (name,isbb,used) values ('", new system().ChkSql(this.tbname.Text.ToString()), "',", num2, ",", num, ")" }));
                base.Response.Redirect("cpitemtype.aspx");
            }
        }

        private void DataGrid1_Cencel(object sender, DataGridCommandEventArgs e)
        {
            this.DataGrid1.EditItemIndex = -1;
            this.DataGrid1.DataSource = new DataProviders().ExecuteSqlDs("select * from web_itemtype", "DataGrid1");
            this.DataGrid1.DataBind();
        }

        private void DataGrid1_Delete(object sender, DataGridCommandEventArgs e)
        {
            int num = int.Parse(e.Item.Cells[0].Text);
            if (new DataProviders().ExecScalarOne("select * from web_item where type=" + num) != "none")
            {
                base.Response.Write("<script language=javascript>alert(\"刪除類型需要將此類型的物品全部刪除,請轉到物品列表\")</script>");
            }
            else
            {
                new DataProviders().ExecuteSql("delete from web_itemtype where id=" + num);
                this.DataGrid1.DataSource = new DataProviders().ExecuteSqlDs("select * from web_itemtype", "DataGrid1");
                this.DataGrid1.DataBind();
            }
        }

        private void DataGrid1_Edit(object sender, DataGridCommandEventArgs e)
        {
            this.DataGrid1.EditItemIndex = e.Item.ItemIndex;
            this.DataGrid1.DataSource = new DataProviders().ExecuteSqlDs("select * from web_itemtype", "DataGrid1");
            this.DataGrid1.DataBind();
        }

        private void DataGrid1_Update(object sender, DataGridCommandEventArgs e)
        {
            int num = int.Parse(((TextBox) e.Item.Cells[3].Controls[0]).Text);
            int num2 = int.Parse(((TextBox) e.Item.Cells[2].Controls[0]).Text);
            string text = ((TextBox) e.Item.Cells[1].Controls[0]).Text;
            int num3 = int.Parse(((TextBox) e.Item.Cells[0].Controls[0]).Text);
            new DataProviders().ExecuteSql(string.Concat(new object[] { "update web_itemtype set name='", text, "',used=", num2, ",isbb=", num, " where id=", num3 }));
            this.DataGrid1.EditItemIndex = -1;
            this.DataGrid1.DataSource = new DataProviders().ExecuteSqlDs("select * from web_itemtype", "DataGrid1");
            this.DataGrid1.DataBind();
        }

        private void InitializeComponent()
        {
            this.DataGrid1.CancelCommand += new DataGridCommandEventHandler(this.DataGrid1_Cencel);
            this.DataGrid1.EditCommand += new DataGridCommandEventHandler(this.DataGrid1_Edit);
            this.DataGrid1.UpdateCommand += new DataGridCommandEventHandler(this.DataGrid1_Update);
            this.DataGrid1.DeleteCommand += new DataGridCommandEventHandler(this.DataGrid1_Delete);
            this.btnadd.Click += new EventHandler(this.btnadd_Click);
            base.Load += new EventHandler(this.Page_Load);
        }

        protected override void OnInit(EventArgs e)
        {
            this.InitializeComponent();
            base.OnInit(e);
        }

        private void Page_Load(object sender, EventArgs e)
        {
            new WebLogic().isadmin();
            new WebLogic().diskf();
            if (!this.Page.IsPostBack)
            {
                this.DataGrid1.DataSource = new DataProviders().ExecuteSqlDs("select * from web_itemtype", "DataGrid1");
                this.DataGrid1.DataBind();
            }
        }
    }
}
