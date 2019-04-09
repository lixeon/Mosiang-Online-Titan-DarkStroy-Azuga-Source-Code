namespace web.admin
{
    using System;
    using System.Data.SqlClient;
    using System.Web.UI;
    using System.Web.UI.WebControls;
    using web;

    public class cpdownload : Page
    {
        protected Button btnadd;
        protected Button btnedit;
        protected CheckBox cbdate;
        protected DataGrid DataGrid1;
        protected Label lblid;
        protected RequiredFieldValidator RequiredFieldValidator1;
        protected RequiredFieldValidator RequiredFieldValidator2;
        protected RequiredFieldValidator RequiredFieldValidator3;
        protected RequiredFieldValidator RequiredFieldValidator4;
        protected TextBox tbcomment;
        protected TextBox tblink;
        protected TextBox tbname;
        protected TextBox tbsize;

        private void btnadd_Click(object sender, EventArgs e)
        {
            string str = new system().ChkSql(this.tbname.Text.ToString());
            string str2 = new system().ChkSql(this.tblink.Text.ToString());
            int num = int.Parse(this.tbsize.Text.ToString());
            string str3 = new system().ChkSql(this.tbcomment.Text.ToString());
            new DataProviders().ExecuteSql(string.Concat(new object[] { "insert into web_download (name,link,comment,size) values ('", str, "','", str2, "','", str3, "',", num, ")" }));
            base.Response.Redirect("cpdownload.aspx");
        }

        private void btnedit_Click(object sender, EventArgs e)
        {
            int num = int.Parse(this.lblid.Text);
            string str = new system().ChkSql(this.tbname.Text.ToString());
            string str2 = new system().ChkSql(this.tblink.Text.ToString());
            int num2 = int.Parse(this.tbsize.Text.ToString());
            string str3 = this.cbdate.Checked ? ",date=getdate()" : "";
            string str4 = new system().ChkSql(this.tbcomment.Text.ToString());
            new DataProviders().ExecuteSql(string.Concat(new object[] { "update web_download set name='", str, "',link='", str2, "',size=", num2, str3, ",comment='", str4, "' where id=", num }));
            this.btnedit.Visible = false;
            this.btnadd.Visible = true;
            base.Response.Redirect("cpdownload.aspx");
        }

        private void DataGrid1_Delete(object sender, DataGridCommandEventArgs e)
        {
            int num = int.Parse(e.Item.Cells[0].Text);
            new DataProviders().ExecuteSql("delete from web_download where id=" + num);
            base.Response.Redirect("cpdownload.aspx");
        }

        private void DataGrid1_SelectedIndexChanged(object sender, EventArgs e)
        {
            int num = int.Parse(this.DataGrid1.SelectedItem.Cells[0].Text.ToString());
            DataProviders providers = new DataProviders();
            SqlDataReader reader = providers.ExecuteSqlDataReader("select * from web_download where id=" + num);
            if (reader.Read())
            {
                this.lblid.Text = num.ToString();
                this.tbname.Text = reader["name"].ToString();
                this.tblink.Text = reader["link"].ToString();
                this.tbsize.Text = reader["size"].ToString();
                this.tbcomment.Text = reader["comment"].ToString();
                this.cbdate.Visible = true;
                this.btnedit.Visible = true;
            }
            reader.Close();
            providers.CloseConn();
        }

        private void InitializeComponent()
        {
            this.DataGrid1.DeleteCommand += new DataGridCommandEventHandler(this.DataGrid1_Delete);
            this.DataGrid1.SelectedIndexChanged += new EventHandler(this.DataGrid1_SelectedIndexChanged);
            this.btnedit.Click += new EventHandler(this.btnedit_Click);
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
                this.DataGrid1.DataSource = new DataProviders().ExecuteSqlDs("select * from web_download order by date desc", "DataGrid1");
                this.DataGrid1.DataBind();
            }
        }
    }
}
