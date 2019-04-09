namespace web.admin
{
    using FredCK.FCKeditorV2;
    using System;
    using System.Data.SqlClient;
    using System.Web.UI;
    using System.Web.UI.WebControls;
    using web;

    public class cpnews : Page
    {
        protected Button btnadd;
        protected Button btnedit;
        protected CheckBox cbdate;
        protected DataGrid DataGrid1;
        protected DropDownList DropDownList1;
        protected FCKeditor edit;
        protected Label lblid;
        protected RequiredFieldValidator RequiredFieldValidator1;
        protected RequiredFieldValidator RequiredFieldValidator2;
        protected TextBox tbauthor;
        protected TextBox tbtitle;

        private void btnadd_Click(object sender, EventArgs e)
        {
            if (this.edit.Value.ToString() == "")
            {
                base.Response.Write("<script language=javascript>alert(\"新聞內容不能為空\")</script>");
            }
            else
            {
                string str = new system().ChkSql(this.tbtitle.Text.ToString());
                int num = int.Parse(this.DropDownList1.SelectedValue);
                string str2 = new system().ChkSql(this.tbauthor.Text.ToString());
                string str3 = new system().ChkSql(this.edit.Value.ToString());
                new DataProviders().ExecuteSql(string.Concat(new object[] { "insert into web_news (title,content,author,type) values ('", str, "','", str3, "','", str2, "',", num, ")" }));
                base.Response.Redirect("cpnews.aspx");
            }
        }

        private void btnedit_Click(object sender, EventArgs e)
        {
            int num = int.Parse(this.lblid.Text);
            string str = new system().ChkSql(this.tbtitle.Text.ToString());
            string str2 = new system().ChkSql(this.edit.Value.ToString());
            string str3 = new system().ChkSql(this.tbauthor.Text.ToString());
            int num2 = int.Parse(this.DropDownList1.SelectedValue);
            string str4 = this.cbdate.Checked ? ",adddate=getdate()" : "";
            string mySql = string.Concat(new object[] { "update web_news set title='", str, "',author='", str3, "',content='", str2, "',type=", num2, str4, " where id=", num });
            new DataProviders().ExecuteSql(mySql);
            this.btnedit.Visible = false;
            this.cbdate.Visible = false;
            this.btnadd.Visible = true;
            base.Response.Redirect("cpnews.aspx");
        }

        private void DataGrid1_Delete(object sender, DataGridCommandEventArgs e)
        {
            int num = int.Parse(e.Item.Cells[0].Text);
            new DataProviders().ExecuteSql("delete from web_news where id=" + num);
            this.DataGrid1.DataSource = new DataProviders().ExecuteSqlDs("select a.id as id,substring(title,1,20) as title ,name,adddate,author,type,clicks from web_news a,web_newstype b where a.type=b.id order by adddate desc ", "DataGrid1");
            this.DataGrid1.DataBind();
        }

        private void DataGrid1_SelectedIndexChanged(object sender, EventArgs e)
        {
            int num = int.Parse(this.DataGrid1.SelectedItem.Cells[0].Text.ToString());
            DataProviders providers = new DataProviders();
            SqlDataReader reader = providers.ExecuteSqlDataReader("select * from web_news where id=" + num);
            if (reader.Read())
            {
                this.lblid.Text = num.ToString();
                this.tbtitle.Text = reader["title"].ToString();
                this.edit.Value = reader["content"].ToString();
                this.tbauthor.Text = reader["author"].ToString();
                this.DropDownList1.SelectedValue = reader["type"].ToString();
                this.btnadd.Visible = false;
                this.btnedit.Visible = true;
                this.cbdate.Visible = true;
                this.btnadd.Visible = true;
            }
            reader.Close();
            providers.CloseConn();
        }

        private void InitializeComponent()
        {
            this.DataGrid1.DeleteCommand += new DataGridCommandEventHandler(this.DataGrid1_Delete);
            this.DataGrid1.SelectedIndexChanged += new EventHandler(this.DataGrid1_SelectedIndexChanged);
            this.btnadd.Click += new EventHandler(this.btnadd_Click);
            this.btnedit.Click += new EventHandler(this.btnedit_Click);
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
            this.tbauthor.Text = this.Session["admin_name"].ToString();
            if (!this.Page.IsPostBack)
            {
                this.DataGrid1.DataSource = new DataProviders().ExecuteSqlDs("select a.id as id,substring(title,1,20) as title ,name,adddate,author,type,clicks from web_news a,web_newstype b where a.type=b.id order by adddate desc ", "DataGrid1");
                this.DataGrid1.DataBind();
                this.DropDownList1.DataSource = new DataProviders().ExecuteSqlDs("select * from web_newstype", "DropDownList1");
                this.DropDownList1.DataTextField = "name";
                this.DropDownList1.DataValueField = "id";
                this.DropDownList1.DataBind();
            }
        }
    }
}
