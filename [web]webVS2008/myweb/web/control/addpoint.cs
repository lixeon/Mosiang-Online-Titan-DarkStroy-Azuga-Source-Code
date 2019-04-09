namespace web.control
{
    using System;
    using System.Data.SqlClient;
    using System.Web.UI;
    using System.Web.UI.WebControls;
    using web;

    public class addpoint : UserControl
    {
        protected Button btnadd;
        protected DropDownList ddchalist;
        protected Label lbldex;
        protected Label lblgengoal;
        protected Label lblpoint;
        protected Label lblsimmak;
        protected Label lblsta;
        protected RegularExpressionValidator RegularExpressionValidator1;
        protected RegularExpressionValidator Regularexpressionvalidator2;
        protected RegularExpressionValidator Regularexpressionvalidator3;
        protected RegularExpressionValidator Regularexpressionvalidator4;
        protected RequiredFieldValidator RequiredFieldValidator1;
        protected RequiredFieldValidator Requiredfieldvalidator2;
        protected RequiredFieldValidator Requiredfieldvalidator3;
        protected RequiredFieldValidator Requiredfieldvalidator4;
        protected RequiredFieldValidator Requiredfieldvalidator6;
        protected TextBox tbdex;
        protected TextBox tbgengoal;
        protected TextBox tbsimmak;
        protected TextBox tbsta;

        private void btnadd_Click(object sender, EventArgs e)
        {
            int chaidx = int.Parse(this.ddchalist.SelectedValue);
            int dex = Math.Abs(int.Parse(this.tbdex.Text.ToString()));
            int simmak = Math.Abs(int.Parse(this.tbsimmak.Text.ToString()));
            int gengoal = Math.Abs(int.Parse(this.tbgengoal.Text.ToString()));
            int sta = Math.Abs(int.Parse(this.tbsta.Text.ToString()));
            string str = new WebLogic().addpoint(base.Session["userid"].ToString(), chaidx, dex, simmak, gengoal, sta);
            base.Response.Write("<script language=javascript>alert('" + str + "')</script>");
        }

        private void ddchalist_SelectedIndexChanged(object sender, EventArgs e)
        {
            int num = int.Parse(this.ddchalist.SelectedValue);
            if (num == 0)
            {
                this.lbldex.Text = "0";
                this.lblsimmak.Text = "0";
                this.lblgengoal.Text = "0";
                this.lblsta.Text = "0";
                this.lblpoint.Text = "0";
                this.tbsimmak.Text = "0";
                this.tbdex.Text = "0";
                this.tbgengoal.Text = "0";
                this.tbsta.Text = "0";
            }
            else
            {
                DataProviders providers = new DataProviders();
                SqlDataReader reader = providers.ExecuteSqlDataReader("select * from mhgame..tb_character WHERE character_idx=" + num);
                if (reader.Read())
                {
                    this.lbldex.Text = reader["CHARACTER_dex"].ToString();
                    this.lblsimmak.Text = reader["CHARACTER_simmak"].ToString();
                    this.lblgengoal.Text = reader["CHARACTER_gengoal"].ToString();
                    this.lblsta.Text = reader["CHARACTER_sta"].ToString();
                    this.lblpoint.Text = reader["CHARACTER_GRADEUPPOINT"].ToString();
                    this.tbsimmak.Text = "0";
                    this.tbdex.Text = "0";
                    this.tbgengoal.Text = "0";
                    this.tbsta.Text = "0";
                    reader.Close();
                    providers.CloseConn();
                }
                else
                {
                    reader.Close();
                    providers.CloseConn();
                }
            }
        }

        private void InitializeComponent()
        {
            this.ddchalist.SelectedIndexChanged += new EventHandler(this.ddchalist_SelectedIndexChanged);
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
            if (base.Session["userid"] == null)
            {
                base.Response.Redirect("login.aspx?returnurl=" + base.Request.ServerVariables["SCRIPT_NAME"]);
            }
            else if (!this.Page.IsPostBack)
            {
                this.ddchalist.DataSource = new DataProviders().ExecuteSqlDs("select character_idx,character_name+'['+convert(varchar,character_money)+'笴栏刽]' as character_name from mhgame..tb_character  where substring(character_name,1,1)!='@' and user_idx=" + base.Session["useridx"].ToString(), "ddchalist");
                this.ddchalist.DataTextField = "character_name";
                this.ddchalist.DataValueField = "character_idx";
                this.ddchalist.DataBind();
                for (int i = 0; i < this.ddchalist.Items.Count; i++)
                {
                    this.ddchalist.Items[i].Text = new system().ConvertToBig5(this.ddchalist.Items[i].Text.ToString(), 950);
                }
                this.ddchalist.Items.Insert(0, new ListItem("請選擇角色", "0"));
            }
        }
    }
}
